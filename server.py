from http.server import BaseHTTPRequestHandler, HTTPServer
import subprocess
import os
import json
from urllib.parse import parse_qs, urlparse
import structlog

class CalculatorHTTPHandler(BaseHTTPRequestHandler):
    CALCULATOR_PATH = os.path.abspath(os.path.join('build', 'app.exe'))
    
    def __init__(self, *args, **kwargs):
        self.logger = structlog.get_logger()
        super().__init__(*args, **kwargs)

    def log_message(self, format, *args):
        # structlog
        self.logger.info(
            "HTTP request",
            client_address=self.client_address[0],
            method=self.command,
            path=self.path,
            status_code=self._status_code if hasattr(self, '_status_code') else None
        )

    def do_GET(self):
        parsed_url = urlparse(self.path)
        self.logger.info("Incoming GET request", path=self.path)
        
        if parsed_url.path != '/calc':
            self._status_code = 404
            self.logger.warning("Path not found", path=self.path)
            return self._send_error(404, "Not Found")
            
        try:
            params = parse_qs(parsed_url.query)
            expression = params.get('expression', [''])[0]
            use_float = params.get('float', ['false'])[0].lower() == 'true'
            
            if not expression:
                self._status_code = 400
                self.logger.warning("Missing expression parameter")
                return self._send_error(400, "Missing expression parameter")
                
            self.logger.info("Calculating expression", expression=expression, use_float=use_float)
            result = self._calculate(expression, use_float)
            self._status_code = 200
            self._send_success(result)
            
        except Exception as e:
            self._status_code = 500
            self.logger.error("Calculator error", error=str(e), exc_info=True)
            self._send_error(500, "Calculator error occurred")

    def do_POST(self):
        self.logger.info("Incoming POST request", path=self.path)
        
        if self.path != '/calc':
            self._status_code = 404
            self.logger.warning("Path not found", path=self.path)
            return self._send_error(404, "Not Found")
            
        content_length = int(self.headers.get('Content-Length', 0))
        if not content_length:
            self._status_code = 500
            self.logger.warning("Empty request body")
            return self._send_error(500, "Empty request body")
            
        try:
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data)
            
            expression = data.get('expression')
            if not expression:
                self._status_code = 400
                self.logger.warning("Missing expression field")
                return self._send_error(400, "Missing expression field")
                
            use_float = data.get('float', False)
            self.logger.info("Calculating expression", expression=expression, use_float=use_float)
            result = self._calculate(expression, use_float)
            self._status_code = 200
            self._send_success(result)
            
        except json.JSONDecodeError as e:
            self._status_code = 500
            self.logger.error("Invalid JSON format", error=str(e))
            self._send_error(500, "Invalid JSON format")
        except Exception as e:
            self._status_code = 500
            self.logger.error("Calculator error", error=str(e), exc_info=True)
            self._send_error(500, "Calculator error")

    def _calculate(self, expression, use_float=False):
        try:
            command = [self.CALCULATOR_PATH]
            if use_float:
                command.append('--float')
                
            self.logger.debug("Starting calculator process", command=command)
            process = subprocess.Popen(
                command,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding='utf-8'
            )
            
            stdout, stderr = process.communicate(input=expression + '\n')
            
            if process.returncode != 0:
                self.logger.error("Calculator failed", 
                                 returncode=process.returncode,
                                 stderr=stderr.strip())
                raise RuntimeError("Calculator failed")
                
            self.logger.debug("Calculator result", result=stdout.strip())
            return stdout.strip()
            
        except FileNotFoundError as e:
            self.logger.error("Calculator not found", error=str(e))
            raise RuntimeError("Calculator not found")
        except subprocess.SubprocessError as e:
            self.logger.error("Calculator execution error", error=str(e))
            raise RuntimeError("Calculator execution error")

    def _send_success(self, result):
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        response = {
            'status': 'success',
            'result': result
        }
        self.logger.info("Sending successful response", response=response)
        self.wfile.write(json.dumps(response).encode('utf-8'))

    def _send_error(self, code, message):
        self.send_response(code)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        response = {
            'status': 'error',
            'error': message
        }
        self.logger.warning("Sending error response", status_code=code, response=response)
        self.wfile.write(json.dumps(response).encode('utf-8'))

def run_server(port=8080):
    from logging_config import configure_logging
    configure_logging()
    logger = structlog.get_logger()
    
    server_address = ('', port)
    httpd = HTTPServer(server_address, CalculatorHTTPHandler)
    logger.info(f'Starting server on port {port}...')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        logger.info("Server stopped by keyboard interrupt")
    except Exception as e:
        logger.error("Server error", error=str(e))
        raise
    finally:
        logger.info("Server shutting down")
        httpd.server_close()
    
if __name__ == '__main__':
    run_server()
