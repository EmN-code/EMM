from flask import Flask, request, jsonify
import subprocess
import threading
from flask_cors import CORS
import argparse
import json
from iotpilot import *

parser = argparse.ArgumentParser(description="IoTPilot config")
parser.add_argument('--max_attempts', type=int, default=3, help='the maximum number of attempts for self-debug')
parser.add_argument('--app_path', type=str, default='../datasets/app.csv', help='user questions')
parser.add_argument('--iscontext', type=bool, default=False, help='iscontext')
parser.add_argument('--isexecution', type=bool, default=False, help='isexecution')
parser.add_argument('--isruntime', type=bool, default=False, help='isruntime')
parser.add_argument('--isruntimedebug', type=bool, default=False, help='isruntimedebug')
parser.add_argument('--device_type', type=str, default="esp32", help='device')
parser.add_argument('--os_type', type=str, default="RIOT", help='embeded os')

parser.add_argument('--mode', type=str, default='SingleAgent', help='SingleAgent or MultiAgent')
parser.add_argument('--model', type=str, default='gpt-4o', help='gpt-4o, gpt-4o-mini, deepseek-coder')

parser.add_argument('--isuserreq', type=bool, default=False, help='isuserreq')
parser.add_argument('--usereq', type=str, default='mqtt app typically need to ...', help='The user requirements for the execution logic')
parser.add_argument('--userproblem', type=str, default="", help='userproblem')
parser.add_argument('--fix_lines', type=str, default="44", help="fix lines")
parser.add_argument('--ismemory', type=bool, default=True, help="ismemory")
parser.add_argument('--username', type=str, default=False, help="username")
args = parser.parse_args()

app = Flask(__name__)
CORS(app)

def run_iotpilot(username, userproblem):

    try:
        args.username = username
        args.userproblem = userproblem
        results = iotpilot_func(args)
        return results
    except Exception as e:
        return str(e)


@app.route('/run', methods=['POST'])
def run():
    data = request.json
    username = data.get('username')
    userproblem = data.get('userproblem')

    if not username or not userproblem:
        return jsonify({'error': 'Username and userproblem are required'}), 400

    print("username:", username)
    result = run_iotpilot(username, userproblem)

    print(result)

    return jsonify({'result': result}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=False, threaded=True)
