import requests
import json
def adicionar():
    xason = {"name": "Braba", "age": "21"}
    headers = {"Content-type":"application/json"}
    r = requests.post('http://localhost:8081/order',data=json.dumps(xason),headers=headers)
    return r.status_code

def check():
    r = requests.get('http://localhost:8081/')
    r.json()
    return r.status_code