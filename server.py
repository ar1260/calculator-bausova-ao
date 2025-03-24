from http.server import BaseHTTPRequestHandler, HTTPServer

class SimpleHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/calc':
            self.send_response(200)
            self.end_headers()
            self.wfile.write(b'OK')
        else:
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b'Not Found')

def run_server():
    server = HTTPServer(('', 8080), SimpleHandler)
    print('Server started on port 8080')
    server.serve_forever()

if __name__ == '__main__':
    run_server()
