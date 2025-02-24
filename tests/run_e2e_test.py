import os
import numpy as np
import subprocess

MIN_ELEM = 0.9
MAX_ELEM = 1.15
ITERS = 3

def shuffleRows(matrix):
  """Applies basic transformations to matrix keeping its determinant."""
  n = matrix.shape[0]
  if (n == 1):
    return matrix

  for _ in range(n):
    row1 = np.random.randint(0, n)
    row2 = row1
    while (row1 == row2):
      row2 = np.random.randint(0, n)

    coef = np.random.uniform(0.1, 0.2)
    for j in range(n):
      matrix[row1][j] += coef * matrix[row2][j]
    return matrix

def generateRandomMatrix(size):
  matrix = np.random.uniform(MIN_ELEM, MAX_ELEM, (size, size))
  matrix = np.triu(matrix) # because of overflows occurring with common matrices
  return shuffleRows(matrix)

def getDeterminantFromNumPy(matrix):
  """Computes the determinant using NumPy."""
  return np.linalg.det(matrix)

def sendTextToDriver(text):
  file_path = os.path.abspath(os.path.dirname(__file__))
  process = subprocess.run(
    [file_path + "/../build/driver/driver"],  # External executable
    input=text, text=True, capture_output=True
  )
  return process

def getDeterminantFromDriver(matrix):
  """Runs an external program that computes the determinant."""
  # Convert matrix to space-separated string format
  matrix_str = str(matrix.shape[0]) + " " + "\n".join(" ".join(f"{val:.6f}" for val in row) for row in matrix) + "\n"
  process = sendTextToDriver(matrix_str)

  if process.returncode != 0:
    raise RuntimeError(f"External program failed: {process.stderr}")

  return float(process.stdout.strip())

# Separate test for size == 0
print(f"matrix.size = 0")
process = sendTextToDriver("0\n")

if (process.returncode == 0 or process.stderr != "Matrix::det(): matrix size must be > 0\n"):
  print(f"Process returned with code = {process.returncode}")
  print(f"Process stderr: {process.stderr}")
  raise RuntimeError(f"❌ Failed test with size = 0")

sizes = [1, 2, 4, 10, 100, 1000]
for size in sizes:
  print(f"\nmatrix.size = {size}")
  for i in range(1, ITERS + 1):
    matrix = generateRandomMatrix(size)

    det_python = getDeterminantFromNumPy(matrix)
    det_external = getDeterminantFromDriver(matrix)

    print(f"Test [{i}/{ITERS}]")
    print(f"Determinant (NumPy): {det_python}")
    print(f"Determinant (External Program): {det_external}")

    # Check if the results are close (floating-point precision issues may arise)
    if np.isclose(det_python, det_external, rtol=1e-2): # Экспериментально установленная точность
      print("✅ Determinants match!")
    else:
      raise RuntimeError(f"❌ Determinants do not match with size = {size}")
