from http.server import BaseHTTPRequestHandler, HTTPServer

class SimpleHandler(BaseHTTPRequestHandler):
     def do_POST(self):
        if self.path != '/calc':
            self.send_response(404)
            self.end_headers()
            self.wfile.write(json.dumps({'error': 'Not found'}).encode())
            return
            
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)

        # parse json
        try:
            data = json.loads(post_data)
            expression = data.get('expression')
            use_float = data.get('use_float', False)

            if not expression:
                self.send_response(400)
                self.end_headers()
                self.wfile.write(json.dumps({'error': 'Missing expression'}).encode())
                return
def run_server():
    server = HTTPServer(('', 8080), SimpleHandler)
    print('Server started on port 8080')
    server.serve_forever()

if __name__ == '__main__':
    run_server()
