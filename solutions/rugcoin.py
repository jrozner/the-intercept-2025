
import serial
import os
import time

os.system('sudo ../firmware/build/_deps/picotool/picotool reboot -f')

time.sleep(3)

def readUntil(file, needle):
    s = b''

    needle = needle.encode()

    while True:
        s += file.read(1)
        if s[-len(needle):] == needle:
            return s.decode()

def sendLine(file, line):
    line = line.encode()
    file.write(line + b'\n');

def sendCommand(file, cmd):
    sendLine(file, cmd)
    readUntil(file, '\n')
    return readUntil(file, '>')

serial = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=1)

h = sendCommand(serial, 'help')
print(h)

h = sendCommand(serial, 'mining')
print(h)

h = sendCommand(serial, 'help')
print(h)

sendLine(serial, 'rugcoin')
h = readUntil(serial, '\n')
h = readUntil(serial, '\n')
h = readUntil(serial, '\n')


def getAnswer(expr):
    import re
    
    # Tokenize: capture optional signs for numbers, standalone * and / operators
    tokens = re.findall(r'[+-]?\d+|[*/]', expr)

    # Start with the first number
    total = int(tokens[0])

    i = 1
    while i < len(tokens):
        token = tokens[i]

        if token in ('*', '/'):
            # Operator: apply to next number
            num = int(tokens[i+1])
            if token == '*':
                total *= num
            elif token == '/':
                total = int(total / num)  # integer division like C
            i += 2
        else:
            # Token is a signed number (+ or - prefix), add to total
            total += int(token)
            i += 1

    return str(total)

while True:
    m = readUntil(serial, '\n')[:-2]
    print('math=', m)
    answer = getAnswer(m)
    h = readUntil(serial, '?');
    print(h)
    sendLine(serial, answer)
    h = readUntil(serial, '\n')
    print('itgot=', h)
    h = readUntil(serial, '\n')
    if 'RugCoin' in h:
        break
    print('response=', h)

print(readUntil(serial, '\n'))
print(readUntil(serial, '\n'))
print(readUntil(serial, '\n'))
print(readUntil(serial, '\n'))
print(readUntil(serial, '\n'))


