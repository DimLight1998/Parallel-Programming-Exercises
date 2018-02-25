import numpy as np

def generate_normal(num_data):
    with open('data.txt', 'w+') as f:
        f.write(str(num_data) + '\n')
        for _ in range(num_data):
            f.write(str(np.random.normal(50, 15)) + '\n')

if __name__ == '__main__':
    generate_normal(16384)
        