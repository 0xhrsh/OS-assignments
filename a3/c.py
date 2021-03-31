import subprocess
subprocess.call(["g++", "a.cpp"]) # OR gcc for c program
tmp=subprocess.call("./a.out 5")
# print("printing result")
# print(tmp)