import json

def main():

    # program_names = [
    #     "RIOT_MQTT",
    #     "RIOT_COAP",
    #     "RIOT_MQTT_SN",
    #     "RIOT_IRQ",
    #     "RIOT_RTC",
    #     "RIOT_UDP",
    #     "RIOT_Thread",
    #     "RIOT_Timer",
    #     "RIOT_Flash",
    #     "RIOT_MMA",
    #     "RIOT_Event",
    #     "RIOT_DHT11",
    #     "RIOT_Warn",
    #     "RIOT_Sched",
    #     "RIOT_MBOX"
    # ]
    program_names = [  
        "RIOT_MQTT",
        "RIOT_COAP",
        # "RIOT_MQTT_SN",
        "RIOT_IRQ",
        # "RIOT_RTC",
        "RIOT_UDP",
        # "RIOT_Thread",
        # "RIOT_Timer",
        # "RIOT_Flash",
        "RIOT_MMA",
        # "RIOT_Event",
        # "RIOT_DHT11",
        # "RIOT_Warn",
        # "RIOT_Sched",
        # "RIOT_MBOX"
    ]

    iters = 15
    method_type = "deepseek-full"

    for program_name in program_names:
        res = ""

        for dir in range(1, iters + 1):
            final_file = "-1_state.json"
            file_path = '../output/'+str(program_name)+'/'+str(dir)+'/'+str(method_type)+'/'+final_file

            try:
                with open(file_path, 'r') as file:
                    file_content = json.load(file)
            except FileNotFoundError:
                print(f"File not found: {file_path}")
                continue  

            iterations = file_content["iterations"]
            res += str(iterations) +','
        print(f"{program_name} | method: {method_type} | res: {res}")

if __name__ == "__main__":
    main()
