import json
from flask import Flask, request, jsonify

import microservice

app = Flask(__name__)


"""
Wrapper method delegating to microservice for a "get" request.
"""
def get(path):
    def _get():
        code, text = microservice.get(path)
        return jsonify(json.loads(text)), code
    _get.__name__ = "get_"+path
    return _get


"""
Wrapper method delegating to microservice for a "post" request.
"""
def post(path):
    def _post(path):
        if not request.is_json:
            return "Bad request!", 400
        req_text = request.get_data(as_text=True)
        code, text = microservice.post(path, req_text)
        return jsonify(json.loads(text)), code
    return lambda: _post(path)


# Load all "get" routes.
for endpoint in microservice.get_resources:
    handler = get(endpoint)
    app.route(endpoint)(handler)

    
# Load all "post" routes.
handlers = []

for endpoint in microservice.post_resources:
    handler = post(endpoint)
    app.route(endpoint, methods=["POST"])(handler)


# Run the app!
app.run("localhost", 8080)
