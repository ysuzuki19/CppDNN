### prepare the data ###################################
from tensorflow.keras.datasets import mnist
import numpy as np
from PIL import Image
from tqdm import tqdm

(x_train, y_train), (x_test, y_test) = mnist.load_data()
########################################################

datasize_ratio = 10000
datasize_train = x_train.size // datasize_ratio
datasize_test = x_test.size // datasize_ratio

for i in tqdm(range(datasize_train), ascii=True, desc='loading train dataset'):
    with open('./train/{0}.csv'.format(i), mode='w') as f:
        for j in range(x_train.shape[1]):
            for k in range(x_train.shape[2]):
                f.write(str(x_train[i][j][k]))
                if k != x_train.shape[1]-1:
                    f.write(', ')
            f.write('\n')
with open('./train/y.csv', mode='w') as f:
    for i in tqdm(range(datasize_train), ascii=True, desc='saving train dataset'):
        f.write(str(y_train[i]) + ', ')

for i in tqdm(range(datasize_test), ascii=True, desc='loading test dataset'):
    with open('./test/{0}.csv'.format(i), mode='w') as f:
        for j in range(x_test.shape[1]):
            for k in range(x_test.shape[2]):
                f.write(str(x_test[i][j][k]))
                if k != x_test.shape[1]-1:
                    f.write(', ')
            f.write('\n')
with open('./test/y.csv', mode='w') as f:
    for i in tqdm(range(datasize_test), ascii=True, desc='saving test dataset'):
        f.write(str(y_test[i]) + ', ')


''' # following part is save *.png
for i in tqdm(range(datasize_train), ascii=True, desc='loading train dataset'):
    pil_img = Image.fromarray(x_train[i])
    pil_img.save('./train/{0}.png'.format(i))
with open('./train/y.csv', mode='w') as f:
    for i in tqdm(range(datasize_train), ascii=True, desc='saving train dataset'):
        f.write(str(y_train[i]))

for i in tqdm(range(datasize_test), ascii=True, desc='loading test dataset'):
    pil_img = Image.fromarray(x_test[i])
    pil_img.save('./test/{0}.png'.format(i))
with open('./test/y.csv', mode='w') as f:
    for i in tqdm(range(datasize_test), ascii=True, desc='saving test dataset'):
        f.write(str(y_test[i]))
'''
