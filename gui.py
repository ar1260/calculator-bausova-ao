import sys
import json
import requests
from PySide6.QtWidgets import (
    QApplication,
    QWidget,
    QVBoxLayout,
    QLineEdit,
    QPushButton,
    QCheckBox,
    QLabel,
    QMessageBox,
)
from PySide6.QtCore import Qt


class CalculatorClient(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("calc")
        self.setGeometry(100, 100, 300, 200)

        layout = QVBoxLayout()

        self.expression_input = QLineEdit(self)
        self.expression_input.setPlaceholderText("enter expression")
        self.expression_input.textChanged.connect(self.update_button_state)
        layout.addWidget(self.expression_input)

        self.float_mode_checkbox = QCheckBox("use float", self)
        layout.addWidget(self.float_mode_checkbox)

        self.submit_button = QPushButton("send", self)
        self.submit_button.setEnabled(False)
        self.submit_button.clicked.connect(self.send_request)
        layout.addWidget(self.submit_button)

        self.result_label = QLabel("result: ", self)
        self.result_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.result_label)

        self.setLayout(layout)

    def update_button_state(self):
        self.submit_button.setEnabled(bool(self.expression_input.text().strip()))

    def send_request(self):
        expression = self.expression_input.text().strip()
        if not expression:
            QMessageBox.warning(self, "error", "empty expression!!!")
            return

        use_float = self.float_mode_checkbox.isChecked()

        data = {
            "expression": expression,
            "float": use_float
        }

        try:
            response = requests.post(
                "http://localhost:8080/calc",
                json=data,
                headers={'Content-Type': 'application/json'},
                timeout=5
            )

            if response.status_code == 200:
                try:
                    result_data = response.json()
                    if result_data.get("status") == "success":
                        self.result_label.setText(f"result: {result_data['result']}")
                    else:
                        QMessageBox.warning(self, "error", 
                                         f"server status: {result_data.get('status', 'unknown')}")
                except json.JSONDecodeError:
                    QMessageBox.critical(self, "error", "response parse error")
            else:
                try:
                    error_data = response.json()
                    error_msg = error_data.get('error', response.text)
                    QMessageBox.critical(self, "error", 
                                      f"server error ({response.status_code}): {error_msg}")
                except json.JSONDecodeError:
                    QMessageBox.critical(self, "error", 
                                      f"server error ({response.status_code}): {response.text}")

        except requests.exceptions.RequestException as e:
            QMessageBox.critical(self, "error", f"connect error: {str(e)}")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    client = CalculatorClient()
    client.show()
    sys.exit(app.exec())
