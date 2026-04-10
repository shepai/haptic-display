import serial
import serial.tools.list_ports
import numpy as np
import time


class ArduinoSerial:
    def __init__(self, com_port=None, baudrate=115200, timeout=1):
        self.baudrate = baudrate
        self.timeout = timeout
        self.ser = None

        if com_port is None:
            com_port = self._auto_detect_port()

        self.com_port = com_port
        self._connect()

    def _auto_detect_port(self):
        """
        Try to find an Arduino-like device automatically.
        Falls back to first available port.
        """
        ports = list(serial.tools.list_ports.comports())

        if not ports:
            raise RuntimeError("No serial ports found.")

        # Prefer ports that look like Arduino
        for p in ports:
            desc = (p.description or "").lower()
            if "arduino" in desc or "ch340" in desc or "usb serial" in desc:
                return p.device

        # fallback
        return ports[0].device

    def _connect(self):
        """Open serial connection."""
        self.ser = serial.Serial(
            self.com_port,
            self.baudrate,
            timeout=self.timeout
        )
        time.sleep(2)  # allow Arduino reset after connection

    def close(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def reset(self):
        """
        Sends reset command to Arduino.
        You must implement handling on Arduino side.
        """
        self._send_line("RESET")

    def convert_image(self, matrix):
        """
        Takes a 10x10 matrix, normalizes it to [0,1],
        flattens it, and sends it over serial.
        """

        arr = np.array(matrix, dtype=float)

        if arr.shape != (10, 10):
            raise ValueError("Input must be a 10x10 matrix.")

        # normalize to 0–1
        min_val = np.min(arr)
        max_val = np.max(arr)

        if max_val - min_val == 0:
            norm = np.zeros_like(arr)
        else:
            norm = (arr - min_val) / (max_val - min_val)

        flat = norm.flatten()

        # convert to comma-separated string
        payload = ",".join(f"{v:.5f}" for v in flat)

        self._send_line(payload)

    def _send_line(self, text):
        """Send a line to Arduino."""
        if not self.ser or not self.ser.is_open:
            raise RuntimeError("Serial connection not open.")

        self.ser.write((text + "\n").encode("utf-8"))

    def read_line(self):
        """Optional helper to read response."""
        if self.ser and self.ser.in_waiting:
            return self.ser.readline().decode(errors="ignore").strip()
        return None
    
if __name__=="__main__":
    arduino = ArduinoSerial()  # auto-detect COM port

    matrix = [[i + j for j in range(10)] for i in range(10)]
    arduino.convert_image(matrix)

    arduino.reset()

    arduino.close()