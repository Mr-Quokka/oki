import os
import random

def createRepertory(i):
    os.mkdir(f'TestGeneration/package_{i}')
    os.mkdir(f'TestGeneration/package_{i}/src/')


def createFiles(i):
    for j in range(3):
        file = open(f"TestGeneration/package_{i}/src/file_{j}.c","a")
        file.write("Je suis un fichier C")

def createTomlFile(i):
    name = 'package_'+ str(i)
    version = '0.0.1'
    description = 'Package to test resolution algorithm'
    license = 'MIT'

    file = open(f"TestGeneration/package_{i}/oki.toml","a")
    file.write(
        "[pacakage]\n" + 
        "name = " + '"' + name + '"\n' + 
        "version = " + '"' + version + '"\n' + 
        "description = " + '"' + description + '"\n' + 
        "license = " + '"' + license + '"\n' +

        "[dependencies]\n" +
        "package_" + random.randint(0,4) + " = " + version
        )

for i in range(5):
    createRepertory(i)
    createFiles(i)
    createTomlFile(i)