from http.server import BaseHTTPRequestHandler, HTTPServer
import subprocess
import os
import json

class CalculatorHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path != '/calc':
            self.send_response(404)
            self.end_headers()
            return

        try:
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data)

            expression = data.get('expression')
            if not expression:
                self.send_response(400)
                self.end_headers()
                return

            use_float = data.get('use_float', False)
            result = self._calculate(expression, use_float)
            
            self.send_response(200)
            self.end_headers()
            self.wfile.write(result.encode())

        except Exception:
            self.send_response(500)
            self.end_headers()

    def _calculate(self, expression, use_float):
        """Вызов внешнего калькулятора"""
        cmd = ['./build/app.exe', '--float'] if use_float else ['./build/app.exe']
        process = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = process.communicate(expression + '\n')
        
        if process.returncode != 0:
            raise RuntimeError(stderr)
        return stdout.strip()

def run_server():
    server = HTTPServer(('', 8080), CalculatorHandler)
    print("Server running on port 8080")
    server.serve_forever()

if __name__ == '__main__':
    run_server()
