#!/usr/bin/env python3

import sys

file = sys.argv[1]

lines = [line.strip() for line in open(file).readlines()]

counter = {}
x, y = 0, 0
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
    idx = 10000*x+y
    counter[idx] = 1

x, y = 0, 0
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
    idx = 10000*x+y
    if idx in counter:
      dist = abs(x) + abs(y)
      if dist < mindist or mindist < 0:
        mindist = dist

print("Minimal Manhattan distance:", mindist)

