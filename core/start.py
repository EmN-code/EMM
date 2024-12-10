from single_agent import *
from multi_agent import *
import argparse


parser = argparse.ArgumentParser(description="IoTPilot config")
parser.add_argument('--max_attempts', type=int, default=10, help='the maximum number of attempts for self-debug')
parser.add_argument('--app_path', type=str, default='../datasets/app.csv', help='user questions')
parser.add_argument('--iscontext', type=bool, default=True, help='iscontext')
parser.add_argument('--isexecution', type=bool, default=False, help='isexecution')
parser.add_argument('--isruntime', type=bool, default=False, help='isruntime')
parser.add_argument('--isruntimedebug', type=bool, default=False, help='isruntimedebug')
parser.add_argument('--device_type', type=str, default="esp32", help='device')
parser.add_argument('--os_type', type=str, default="Contiki", help='embeded os')

parser.add_argument('--mode', type=str, default='SingleAgent', help='SingleAgent or MultiAgent')
parser.add_argument('--model', type=str, default='deepseek-coder', help='gpt-4o, gpt-4o-mini, deepseek-coder')

parser.add_argument('--isuserreq', type=bool, default=False, help='')
parser.add_argument('--usereq', type=str, default='mqtt app typically need to ...', help='')

args = parser.parse_args()


def main(args=args):
    process_ids = 16
    data_dict = data_loader(args.app_path)
    code_names = [  
        # "Contiki_DHT11",
        # "Contiki_led",
        # "Contiki_UDP",
        # "Contiki_COAP",
        "Contiki_MQTT",
    ]
    # code_names = [  
        # "Zephyr_MQTT",
        # "Zephyr_COAP",
        # "RIOT_MQTT_SN",
        # "Zephyr_IRQ",
        # "RIOT_RTC",
        # "Zephyr_UDP",
        # "RIOT_Thread",
        # "RIOT_Timer",
        # "RIOT_Flash",
        # "Zephyr_MMA",
        # "RIOT_Event",
        # "RIOT_DHT11",
        # "RIOT_Warn",
        # "RIOT_Sched",
        # "RIOT_MBOX"
    # ]
    # code_names = [  
    #     "RIOT_MQTT",
    #     "RIOT_COAP",
    #     # "RIOT_MQTT_SN",
    #     "RIOT_IRQ",
    #     # "RIOT_RTC",
    #     "RIOT_UDP",
    #     # "RIOT_Thread",
    #     # "RIOT_Timer",
    #     # "RIOT_Flash",
    #     "RIOT_MMA",
    #     # "RIOT_Event",
    #     # "RIOT_DHT11",
    #     # "RIOT_Warn",
    #     # "RIOT_Sched",
    #     # "RIOT_MBOX"
    #     ]
    threads = []
    max_attempts = args.max_attempts
    iscontext = args.iscontext
    isexecution = args.isexecution
    isruntime = args.isruntime
    isruntimedebug = args.isruntimedebug
    isuserreq = args.isuserreq
    usereq = args.usereq
    device_type = args.device_type
    os_type = args.os_type

    # with open("logicprompt.txt", 'r', encoding='utf-8') as file:
    #     usereq = file.read()
    mode = args.mode
    model = args.model
    method_type = "deepseeker-wo.execution"

    for _, code_name in enumerate(code_names):
        # print(f"{code_name} start generation>>>")
        print(data_dict[code_name])
        thread = None
        if mode == "SingleAgent":
            role = -1
            for process_id in range(1, 6):
                thread = threading.Thread(target=single_agent, args=(process_id, model, data_dict[code_name], \
                    max_attempts, iscontext, isexecution, isruntime, isruntimedebug, isuserreq, usereq, code_name, role, method_type, device_type, os_type))
                threads.append(thread)
                thread.start()  
            for thread in threads:
                thread.join()

        elif mode == "MultiAgent":
            role = -1
            for process_id in range(1, process_ids):
                thread = threading.Thread(target=multi_agent, args=(process_id, model, data_dict[code_name], max_attempts, \
                    iscontext, isexecution, isruntime, isuserreq, usereq, code_name, role))
                threads.append(thread)
                thread.start()  
            for thread in threads:
                thread.join()
        else:
            print("invalid mode argument")


if __name__ == "__main__":
    main()


