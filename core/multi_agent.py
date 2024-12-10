from libs import *
import threading

def outline_agent(model, question, process_id):
    modules = outline(model, question)
    print(modules)
    return modules

def code_agents(process_id, model, modules, max_attempts, iscontext, isexecution, isruntime, isuserreq, usereq, program_name, role):

    sub_solutions = []
    for count, question in enumerate(modules, start=1):
        print(question)
        state = CodeState()
        iscontext = True
        attempts = 0
        whitelist = []
        blacklist = []
        correct_lists = []
        while state.errorstate != "pass" and attempts <= max_attempts:
            attempts += 1
            vectorstore = None
            code_gen(process_id, model, state, vectorstore, question, iscontext, isexecution, isuserreq, usereq, \
                    whitelist, blacklist, correct_lists, program_name, count)
            compiler(state, "Esp32", "RIOT")
            state.to_json(state.iterations, count, program_name, process_id)

            if state.errorstate == "pass":
                sub_solutions.append(state.generation)
                break

    return sub_solutions

def intergration_agent(process_id, program_name, model, question, sub_solutions, max_attempts, role):
    state = CodeState()
    iscontext = True
    attempts = 0
    while state.errorstate != "pass" and attempts <= max_attempts:
        attempts += 1
        intergration_gen(process_id, program_name, model, state, question, sub_solutions, iscontext, role)
        compiler(state, "Esp32", "RIOT")
        state.to_json(state.iterations, 1, program_name, process_id)

def multi_agent(process_id, model,question, max_attempts, iscontext, isexecution, isruntime, isuserreq, usereq, program_name, role):
    modules = outline_agent(model, question, process_id)
    sub_solutions = code_agents(process_id, model, modules, max_attempts, iscontext, isexecution, isruntime, isuserreq, usereq, program_name, role)
    intergration_agent(process_id, program_name, model, question, sub_solutions, max_attempts, role)






        




