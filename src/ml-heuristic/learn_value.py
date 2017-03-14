'''
A logistic regression learning algorithm example using TensorFlow library.
This example is using the MNIST database of handwritten digits
(http://yann.lecun.com/exdb/mnist/)
Author: Aymeric Damien
Project: https://github.com/aymericdamien/TensorFlow-Examples/
'''

from __future__ import print_function

import tensorflow as tf
import random

print('Loading Data...')
file_path = './value_training_data.txt'
data = []
data_in = []
data_out = []
with open(file_path, 'r') as f:
    for ln in f.readlines():
        s = ln.split(' ')
        try:
            data.append( ([int(x) for x in s[0]], [int(s[1])/64.0]) )
        except:
            print (ln)
        if random.random() > 2:
            break
    print('Shuffling Data...')
    random.shuffle(data)
    data_in = [x[0] for x in data]
    data_out = [x[1] for x in data]

print('Done Loading Data!')

# Parameters
learning_rate = 0.01
training_epochs = 101
batch_size = 100
display_step = 1

# tf Graph Input
x = tf.placeholder(tf.float32, [None, 192]) # mnist data image of shape 28*28=784
y = tf.placeholder(tf.float32, [None, 1]) # 0-9 digits recognition => 10 classes

# Set model weights
W1 = tf.Variable(tf.random_normal([192, 192]))
W2 = tf.Variable(tf.random_normal([192, 192]))
W3 = tf.Variable(tf.random_normal([192, 1]))
b1 = tf.Variable(tf.random_normal([192]))
b2 = tf.Variable(tf.random_normal([192]))
b3 = tf.Variable(tf.random_normal([1]))

# Construct model
pred = tf.tanh(tf.matmul(
        tf.tanh(tf.matmul(
            tf.nn.tanh(tf.matmul(x, W1) + b1),
            W2
        ) + b2),
        W3
    ) + b3)

# Minimize error using cross entropy
cost = tf.reduce_mean(tf.reduce_sum(tf.pow(y - pred,2), reduction_indices=1))
# Gradient Descent
optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(cost)

saver = tf.train.Saver(tf.global_variables())

# Initializing the variables
init = tf.global_variables_initializer()

# Launch the graph
with tf.Session() as sess:
    sess.run(init)

    # Training cycle
    for epoch in range(training_epochs):
        avg_cost = 0.
        total_batch = len(data)/batch_size
        # Loop over all batches
        for i in range(total_batch):
            ix = i * batch_size
            batch_xs = data_in[ix:ix+batch_size]
            batch_ys = data_out[ix:ix+batch_size];
            # Run optimization op (backprop) and cost op (to get loss value)
            _, c = sess.run([optimizer, cost], feed_dict={x: batch_xs,
                                                          y: batch_ys})
            # Compute average loss
            avg_cost += c
            #print("cost=",c," avg=",avg_cost/i)
        # Display logs per epoch step
        if (epoch+1) % display_step == 0:
            print("Epoch:", '%04d' % (epoch+1), "cost=", "{:.9f}".format(avg_cost/total_batch))
        saver.save(sess, 'value_net', global_step=epoch)
        learning_rate = learning_rate * 0.97

    print("Optimization Finished!")

    # Test model
    correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(y, 1))
    # Calculate accuracy
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
