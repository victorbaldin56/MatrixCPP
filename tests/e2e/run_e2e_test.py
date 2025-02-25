import config
import os
import numpy as np
import subprocess

def getDeterminantFromNumPy(matrix):
  """Computes the determinant using NumPy."""
  return np.linalg.det(matrix)

def sendFileToDriver(file):
  with open(file, 'r') as infile:
    file_path = os.path.abspath(os.path.dirname(__file__))
    process = subprocess.run(
      [file_path + "/../../build/driver/driver"],  # External executable
      stdin=infile, text=True, capture_output=True
    )
    return process

def getDeterminantFromDriver(input_file):
  """Runs an external program that computes the determinant."""
  # Convert matrix to space-separated string format
  process = sendFileToDriver(input_file)

  if process.returncode != 0:
    raise RuntimeError(f"External program failed: {process.stderr}")

  return float(process.stdout.strip())

def getAns(file):
  with open(file, 'r') as f:
    return float(f.readline().strip())

for i in range(config.NUM_TESTS):
  input_file = config.input_dir + f"test_{i + 1}.in"
  ans_file = config.ans_dir + f"ans_{i + 1}.out"
  det_external = getDeterminantFromDriver(input_file)
  det_python = getAns(ans_file)

  print(f"Test [{i+1}/10]")
  print(f"Determinant (External Program): {det_external}")
  print(f"Determinant (reference): {det_python}")

  # Check if the results are close (floating-point precision issues may arise)
  if np.isclose(det_python, det_external, rtol=5e-3): # Экспериментально установленная точность
    print("✅ Determinants match!")
  else:
    raise RuntimeError(f"❌ Determinants do not match")
