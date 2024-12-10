from libs import *
import threading

def single_agent(process_id, model, question, max_attempts, iscontext, isexecution, isruntime, isruntimedebug, isuserreq, usereq, \
                  program_name, role, method_type, device_type, os_type):
    compile_state = CodeState()
    vectorstore = None
    attempts = 0
    correct_lists = []
    whitelist = []
    blacklist = []
    
    # Generate the code with correct headers and apis (context-aware and execution-aware)
    while compile_state.errorstate != "pass" and attempts <= max_attempts:
        attempts += 1
        code_gen(process_id, model, compile_state, vectorstore, question, iscontext, isexecution, isuserreq, usereq, \
                 whitelist, blacklist, correct_lists, program_name, role, method_type, device_type, os_type)
        compiler(compile_state, device_type, os_type)
        compile_state.to_json(compile_state.iterations, -1, program_name, process_id, method_type)
        

    if isruntime:
        executor_state = CodeState()
        # Generate the code with FSMs and runtimelogs (execution-aware)
        while executor_state.errorstate != "pass" and attempts <= max_attempts:
            executor_state.generation = compile_state.generation
            if isruntimedebug:
                injected_code_gen(model, executor_state, question)
            compiler(executor_state, "Esp32", "RIOT")
            if executor_state.errorstate == "pass":
                executor(executor_state, 10)
                if isruntimedebug:
                    refine_code(model, executor_state, question)
                executor_state.to_json(executor_state.iterations, -1, program_name, )
    # else:
    #     print("Code generation end")

    return compile_state.generation
        




        




