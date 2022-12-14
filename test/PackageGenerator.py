import os
import random

def createRepertory(i):
    os.mkdir(f'testPackages/package_{i}')
    os.mkdir(f'testPackages/package_{i}/src/')
    os.mkdir(f'testPackages/package_{i}/test/')


def createSourcesFiles(i):
    for j in range(3):
        file = open(f"testPackages/package_{i}/src/file_{j}.c","a")
        file.write("Je suis un fichier C")

def createTestsFiles(i):
    file = open(f"testPackages/package_{i}/test/test.c","a")
    file.write("Je suis un fichier C de test")

def createTomlFile(i):
    name = 'package_'+ str(i)
    version = '0.0.1'
    description = 'Package to test resolution algorithm'
    license = 'MIT'
    dependency = random.randint(0,4)

    while dependency == i :
        dependency = random.randint(0,4)
    

    file = open(f"testPackages/package_{i}/oki.toml","a")
    file.write(
        "[pacakage]\n" +
        "name = " + '"' + name + '"\n' + 
        "version = " + '"' + version + '"\n' + 
        "description = " + '"' + description + '"\n' + 
        "license = " + '"' + license + '"\n\n' +

        "[dependencies]\n" +
        "package_" + str(dependency) + " = " + '"' + version + '"'
        )

for i in range(1,6):
    createRepertory(i)
    createSourcesFiles(i)
    createTestsFiles(i)
    createTomlFile(i)