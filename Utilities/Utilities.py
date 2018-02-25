import numpy as np

debug_folder = '../Debug/'

def generate_normal(path, num_data):
    with open(path, 'w+') as f:
        f.write(str(num_data) + '\n')
        for _ in range(num_data):
            f.write(str(np.random.normal(50, 15)) + '\n')

if __name__ == '__main__':
    generate_normal(debug_folder + 'data.txt', 16384)
        