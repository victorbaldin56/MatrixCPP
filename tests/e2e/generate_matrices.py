import config
import os
import numpy as np

MIN_ELEM = 0.9
MAX_ELEM = 1.15
ITERS = 3

def shuffleRows(matrix):
  """Applies basic transformations to matrix keeping its determinant."""
  n = matrix.shape[0]
  if n == 1:
    return matrix

  for _ in range(int(n**1.5)):
    # Randomly select two distinct rows
    row1 = np.random.randint(0, n)
    row2 = row1
    while row1 == row2:
      row2 = np.random.randint(0, n)

    # Ensure row2 is below row1 to break the upper triangular structure
    if row2 < row1:
      row1, row2 = row2, row1

    # Add a random multiple of row1 to row2
    coef = np.random.uniform(0, 1)
    for j in range(n):
      matrix[row2][j] += coef * matrix[row1][j]

  return matrix

def generateRandomMatrix(size):
  matrix = np.random.uniform(MIN_ELEM, MAX_ELEM, (size, size))
  matrix = np.triu(matrix) # because of overflows occurring with common matrices
  return shuffleRows(matrix)

def dumpMatrixToFile(matrix, file_path):
  matrix_str = str(matrix.shape[0]) + " " + "\n".join(" ".join(f"{val:.6f}" for val in row) for row in matrix) + "\n"
  with open(file_path, 'w') as f:
    f.write(matrix_str)

def dumpAnsToFile(ans, file_path):
  with open(file_path, 'w') as f:
    f.write(str(ans) + '\n')

file_path = os.path.abspath(os.path.dirname(__file__))
if not os.path.isdir(config.input_dir):
  os.mkdir(config.input_dir)

if not os.path.isdir(config.ans_dir):
  os.mkdir(config.ans_dir)

for i in range(config.NUM_TESTS):
  size = np.random.randint(100, 1000)
  print(f"matrix.size = {size}\n")
  matrix = generateRandomMatrix(size)
  det_python = np.linalg.det(matrix)
  input_path = config.input_dir + f"test_{i + 1}.in"
  ans_path = config.ans_dir + f"ans_{i + 1}.out"
  dumpMatrixToFile(matrix=matrix, file_path=input_path)
  dumpAnsToFile(ans=det_python, file_path=ans_path)
