from single_agent import *
from multi_agent import *
import argparse
import json


parser = argparse.ArgumentParser(description="IoTPilot config")
parser.add_argument('--max_attempts', type=int, default=5, help='the maximum number of attempts for self-debug')
parser.add_argument('--app_path', type=str, default='../datasets/app.csv', help='user questions')
parser.add_argument('--iscontext', type=bool, default=False, help='iscontext')
parser.add_argument('--isexecution', type=bool, default=False, help='isexecution')
parser.add_argument('--isruntime', type=bool, default=False, help='isruntime')
parser.add_argument('--isruntimedebug', type=bool, default=False, help='isruntimedebug')
parser.add_argument('--device_type', type=str, default="esp32", help='device')
parser.add_argument('--os_type', type=str, default="RIOT", help='embeded os')

parser.add_argument('--mode', type=str, default='SingleAgent', help='SingleAgent or MultiAgent')
parser.add_argument('--model', type=str, default='deepseek-coder', help='gpt-4o, gpt-4o-mini, deepseek-coder')

parser.add_argument('--isuserreq', type=bool, default=False, help='isuserreq')
parser.add_argument('--usereq', type=str, default='mqtt app typically need to ...', help='The user requirements for the execution logic')
parser.add_argument('--userproblem', type=str, default="", help='userproblem')
parser.add_argument('--fix_lines', type=str, default="44", help="fix lines")
parser.add_argument('--ismemory', type=bool, default=True, help="ismemory")
parser.add_argument('--username', type=str, default="", help="username")
args = parser.parse_args()


def iotpilot_func(args=args):
    
    max_attempts = args.max_attempts
    iscontext = args.iscontext
    isexecution = args.isexecution
    isruntime = args.isruntime
    isruntimedebug = args.isruntimedebug
    isuserreq = args.isuserreq
    usereq = args.usereq
    mode = args.mode
    model = args.model
    userproblem = args.userproblem
    fix_lines = args.fix_lines
    device_type = args.device_type
    os_type = args.os_type
    ismemory = args.ismemory
    method_type = "user"
    codename = args.username

    print("start generation>>>", userproblem)
    if mode == "SingleAgent":
        role = -1
        process_id = "test"
        if ismemory:
            file_path = '/home/kaikai/IoTPilotV2/output/emm/test/user'+'/-1_state.json'
            print(file_path)
            if os.path.isfile(file_path):
                with open(file_path, 'r', encoding='utf-8') as file:
                    curr_state = json.load(file)
                current_code = curr_state['generation']
                print(userproblem)
                userproblem = "current code: " + current_code + "You only need to modify the current code line " + fix_lines + " by" + userproblem
                # userproblem = "current code: " + current_code + "You only need to modify the current code" + " by " + userproblem
        print(userproblem)  
        gencode = single_agent(process_id, model, userproblem, \
            max_attempts, iscontext, isexecution, isruntime, isruntimedebug, isuserreq, usereq, codename, role, method_type, device_type, os_type)
        return gencode
    
    elif mode == "MultiAgent":
        multi_agent(model, userproblem, max_attempts, \
            iscontext, isexecution, isruntime, isuserreq, usereq, codename, 1)
    else:
        print("invalid mode argument")


if __name__ == "__main__":
    iotpilot_func()


