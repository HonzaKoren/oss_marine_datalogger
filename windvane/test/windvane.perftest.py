#!/usr/bin/env python3

import argparse
import requests
from time import time
import sys

parser = argparse.ArgumentParser(
                    prog = 'Windvane performance test',
                    description = 'HTTP load generator fetching data with no delay for defined amount of times',
                    epilog = 'Uses python requests library')
parser.add_argument('-c', '--count', type=int, default=1000)
parser.add_argument('-d', '--destination', type=str, default='172.16.100.34')
args = parser.parse_args()

count = 0
limit = args.count

start = time()
while count < limit:
    r = requests.get(f'http://{args.destination}/metrics/')
    if r.status_code != 200:
        print(f'Request failed with status {r.status_code}')
    count += 1
end = time()

print(f'Test took {end - start} seconds, average request duration {1000 * (end - start) / limit} ms.')
