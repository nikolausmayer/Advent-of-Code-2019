use std::env;
use std::io::BufReader;
use std::io::BufRead;
use std::fs::File;

fn eval_module_mass(module_mass: i32) -> i32 {
  let mut fuel = module_mass / 3 - 2;
  let mut fuel_for_fuel = fuel;
  loop {
    fuel_for_fuel = fuel_for_fuel / 3 - 2;
    if fuel_for_fuel < 0 {
      break;
    }
    fuel += fuel_for_fuel;
  }
  return fuel;
}

fn main() {
  let args: Vec<String> = env::args().collect();

  let f = File::open(&args[1]).unwrap();
  let file = BufReader::new(&f);

  let mut total_fuel: i32 = 0;

  for line in file.lines() {
    let module_mass: i32 = line.unwrap().parse::<i32>().unwrap();
    let this_module_fuel: i32 = eval_module_mass(module_mass);
    //println!("{} -> {}", module_mass, this_module_fuel);
    total_fuel += this_module_fuel;
  }

  println!("Total fuel required: {}", total_fuel);
}

