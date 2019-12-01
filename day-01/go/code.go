package main

import (
  "bufio"
  "fmt"
  "log"
  "os"
  "strconv"
)

func main() {
  args := os.Args

  file, err := os.Open(args[1])
  if err != nil { log.Fatal(err) }
  defer file.Close()

  lines := bufio.NewScanner(file)
  total_fuel := 0
  for lines.Scan() {
    module_mass, err := strconv.Atoi(lines.Text())
    if err != nil { log.Fatal(err) }

    fuel := module_mass / 3 - 2
    fuel_for_fuel := fuel
    for {
      fuel_for_fuel = fuel_for_fuel / 3 - 2
      if fuel_for_fuel <= 0 {
        break
      }
      fuel += fuel_for_fuel
    }

    fmt.Println(module_mass, "->", fuel)
    total_fuel += fuel
  }

  fmt.Println("Total fuel:", total_fuel)
}

