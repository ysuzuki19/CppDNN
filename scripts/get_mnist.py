from tensorflow.keras.datasets import mnist
import numpy as np
from PIL import Image
from tqdm import tqdm
import os

save_dir = '../dataset'

try:
    os.mkdir(save_dir+'/train')
except FileExistsError:
    pass
try:
    os.mkdir(save_dir+'/test')
except FileExistsError:
    pass

(x_train, y_train), (x_test, y_test) = mnist.load_data()

datasize_ratio = 100000
datasize_train = x_train.size // datasize_ratio
datasize_test = x_test.size // datasize_ratio

for i in tqdm(range(datasize_train), ascii=True, desc='loading train dataset'):
    with open(save_dir+'/train/{0}.csv'.format(i), mode='w') as f:
        for j in range(x_train.shape[1]):
            for k in range(x_train.shape[2]):
                f.write(str(x_train[i][j][k]))
                if k != x_train.shape[1]-1:
                    f.write(', ')
            f.write('\n')
with open(save_dir+'/train/y.csv', mode='w') as f:
    for i in tqdm(range(datasize_train), ascii=True, desc='saving train dataset'):
        f.write(str(y_train[i]) + ', ')

for i in tqdm(range(datasize_test), ascii=True, desc='loading test dataset'):
    with open(save_dir+'/test/{0}.csv'.format(i), mode='w') as f:
        for j in range(x_test.shape[1]):
            for k in range(x_test.shape[2]):
                f.write(str(x_test[i][j][k]))
                if k != x_test.shape[1]-1:
                    f.write(', ')
            f.write('\n')
with open(save_dir+'/test/y.csv', mode='w') as f:
    for i in tqdm(range(datasize_test), ascii=True, desc='saving test dataset'):
        f.write(str(y_test[i]) + ', ')

