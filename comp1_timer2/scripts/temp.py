#!/cygdrive/c/Python37/python.exe

import pylab as plt
import datetime as dt
import re, sys, time, serial, atexit
import os, platform
import argparse

def pollCOM():
    comListTry = []
    if "windows" in platform.system().lower():
        comListTry = map(lambda n: "com%i" % n, range(30))
    elif "cygwin" in platform.system().lower():
        comListTry = range(30)
    else:
        comListTry = range(30)
    # print(list(comListTry))
    # sys.exit()

    comListFound = []
    for comTry in comListTry:
        sys.stdout.write("trying com: %s ..." % comTry)
        sys.stdout.flush()
        try:
            com = serial.Serial(comTry)
            comListFound.append(comTry)
            sys.stdout.write("yes\n")
            com.close()
        except KeyboardInterrupt:
            sys.exit()
        except:
            sys.stdout.write("no\n")
            pass
    sys.stdout.write("%r\n" % comListFound)
    sys.stdout.flush()
    return comListFound

def selectCOM():
    comListFound = pollCOM()
    if len(comListFound) == 0:
        sys.stdout.write("no com port\n")
        sys.stdout.flush()
        sys.exit(0)
    elif len(comListFound) == 1:
        port = comListFound[0]
    else:
        sys.stdout.write("%r\n" % comListFound)
        sys.stdout.flush()
        while True:
            port = input("com:")
            if port in comListFound:
                return port


parser = argparse.ArgumentParser(description='Temperature logger')
parser.add_argument("--com", help="com port")
parser.add_argument("--duration", type=int, help="duration in seconds")
args = parser.parse_args()

port = args.com
if args.com is None:
    port = selectCOM()

duration = args.duration
if args.duration is None:
    duration = 60

# com port settings
comSettings = {
    "port" : port,
    "baudrate" : 115200,
    "parity" : serial.PARITY_NONE,
    "stopbits" : serial.STOPBITS_ONE,
    "bytesize" : serial.EIGHTBITS,
    "timeout" : 0.1
}

try:
    com = serial.Serial(**comSettings)
    atexit.register(lambda: [sys.stdout.write("comport %s closed" % port), com.close()])
except Exception as exc:
    print("could not open comport:", port)
    print(exc)
    sys.exit()

stopAt = dt.datetime.now() + dt.timedelta(seconds = duration)
tsamp = []
temps = []
# pattern = re.compile(r".*?=>(?P<tempC>\d+)\n")
pattern = re.compile(r".*?=>(?P<tempC>\d+\.\d+)\n")
while dt.datetime.now() < stopAt:
    try:
        line = com.read(20) # usually 12 chars
        m = pattern.match(line.decode("ascii"))
        if m:
            temp = m.group("tempC")
        else:
            continue
        sys.stdout.write("%s: %s°C\n" % (dt.datetime.now(), temp))
        sys.stdout.flush()
        tsamp.append(time.time())
        temps.append(float(temp))
    except Exception as e:
        sys.stdout.write("%s" % e)
        sys.stdout.flush()

# start of plots
plt.figure()
plt.clf()
plt.rcParams.update({'legend.fontsize':4, 'font.size':4})
plt.grid(True)
plt.plot(tsamp,temps,'r')
plt.ylim([min(min(temps)-5, 0), max(max(temps)+5, 30)])
plt.title("temperature")
plt.legend(['degC'], loc='upper right')

# end of plots
now = dt.datetime.now().strftime("%Y_%m_%d_%Hh_%Mm")
filename = "%s_log.pdf" % (now)
print("saving data to %s\n" % filename)
plt.savefig("%s" % filename)

