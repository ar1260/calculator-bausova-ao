from http.server import BaseHTTPRequestHandler, HTTPServer
import subprocess
import os
import json
from urllib.parse import parse_qs, urlparse

class CalculatorHTTPHandler(BaseHTTPRequestHandler):
    CALCULATOR_PATH = os.path.abspath(os.path.join('build', 'app.exe'))
    
    def do_GET(self):
        parsed_url = urlparse(self.path)
        if parsed_url.path != '/calc':
            return self._send_error(404, "Not Found")
            
        try:
            params = parse_qs(parsed_url.query)
            expression = params.get('expression', [''])[0]
            use_float = params.get('float', ['false'])[0].lower() == 'true'
            
            if not expression:
                return self._send_error(400, "Missing expression parameter")
                
            result = self._calculate(expression, use_float)
            self._send_success(result)
            
        except Exception:
            self._send_error(500, "Calculator error occurred")

    def do_POST(self):
        if self.path != '/calc':
            return self._send_error(404, "Not Found")
            
        content_length = int(self.headers.get('Content-Length', 0))
        if not content_length:
            return self._send_error(500, "Empty request body")
            
        try:
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data)
            
            expression = data.get('expression')
            if not expression:
                return self._send_error(500, "Missing expression field")
                
            use_float = data.get('float', False)
            result = self._calculate(expression, use_float)
            self._send_success(result)
            
        except json.JSONDecodeError:
            self._send_error(500, "Invalid JSON format")
        except Exception:
            self._send_error(500, "Calculator error")

    def _calculate(self, expression, use_float=False):
        try:
            command = [self.CALCULATOR_PATH]
            if use_float:
                command.append('--float')
                
            process = subprocess.Popen(
                command,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding='utf-8'
            )
            
            stdout, _ = process.communicate(input=expression + '\n')
            
            if process.returncode != 0:
                raise RuntimeError("Calculator failed")
                
            return stdout.strip()
            
        except FileNotFoundError:
            raise RuntimeError("Calculator not found")
        except subprocess.SubprocessError:
            raise RuntimeError("Calculator execution error")

    def _send_success(self, result):
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({
            'status': 'success',
            'result': result
        }).encode('utf-8'))

    def _send_error(self, code, message):
        self.send_response(code)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps({
            'status': 'error',
            'error': message
        }).encode('utf-8'))

def run_server(port=8080):
    server_address = ('', port)
    httpd = HTTPServer(server_address, CalculatorHTTPHandler)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()
    
if __name__ == '__main__':
    run_server()
