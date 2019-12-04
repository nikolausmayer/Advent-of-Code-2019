#!/usr/bin/env python3

import sys

file = sys.argv[1]

lines = [line.strip() for line in open(file).readlines()]

counter = {}
x, y = 0, 0
wirelength = 0
for cmd in lines[0].split(","):
  dx, dy = {
    "U": [ 0, 1],
    "D": [ 0,-1],
    "L": [-1, 0],
    "R": [ 1, 0],
  }[cmd[0]]
  for i in range(int(cmd[1:])):
    x += dx
    y += dy
    wirelength += 1
    idx = 10000*x+y
    counter[idx] = wirelength

x, y = 0, 0
wirelength = 0
mindist = -1
for cmd in lines[1].split(","):
  dx, dy = {
    "U": [ 0, 1],
    "D": [ 0,-1],
    "L": [-1, 0],
    "R": [ 1, 0],
  }[cmd[0]]
  for i in range(int(cmd[1:])):
    x += dx
    y += dy
    wirelength += 1
    idx = 10000*x+y
    if idx in counter:
      dist = wirelength + counter[idx]
      if dist < mindist or mindist < 0:
        mindist = dist

print("Minimal signal delay:", mindist)

