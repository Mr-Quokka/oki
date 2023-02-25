import os
import subprocess
from random import randrange

os.putenv('OKI_REPOSITORY', 'http://localhost:8000')
os.putenv('OKI_USERNAME', 'test')
os.putenv('OKI_PASSWORD', 'test')
WORKING_DIRECTORY = '/tmp/test_packages'


def create_directory(package_name: str):
    os.mkdir(f'{WORKING_DIRECTORY}/{package_name}')
    os.mkdir(f'{WORKING_DIRECTORY}/{package_name}/src/')
    os.mkdir(f'{WORKING_DIRECTORY}/{package_name}/test/')


def create_sources_files(package_name: str, package_id: int):
    with open(f'{WORKING_DIRECTORY}/{package_name}/src/{package_id}.c', 'w') as cfile:
        cfile.write(f'int get{package_name}(void)')
        cfile.write('{')
        cfile.write(f'   return {package_id};')
        cfile.write('}')

    with open(f'{WORKING_DIRECTORY}/{package_name}/src/{package_id}.h', 'w') as hfile:
        hfile.write(f'int get{package_id}(void);')


def create_manifest(name: str, package_id: int, version: str):
    description = 'Package to test the resolution algorithm'

    with open(f'{WORKING_DIRECTORY}/{name}/oki.toml', 'w') as file:
        file.write(f"""[package]
name = "{name}"
version = "{version}"
kind = "c"
description = "{description}"
license = "MIT"
        
[dependencies]
""")

        if package_id < 5:
            return

        for _ in range(randrange(package_id) // 5):
            package = f'package_{randrange(package_id)}'
            file.write(f'{package} = "{version}"\n')


def publish_random(package_id: int, version: str = '0.1.0'):
    name = f'package_{package_id}'
    create_directory(name)
    create_sources_files(name, package_id)
    create_manifest(name, package_id, version)
    os.chdir(f'{WORKING_DIRECTORY}/{name}')
    subprocess.run(['oki', 'publish'])


def main():
    os.mkdir(WORKING_DIRECTORY)
    for i in range(10):
        publish_random(i)


if __name__ == '__main__':
    main()
