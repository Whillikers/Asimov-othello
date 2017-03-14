from __future__ import print_function

import tensorflow as tf
import random

print('Loading Data...')
file_path = './policy_training_data.txt'
data = []
data_in = []
data_out = []
with open(file_path, 'r') as f:
    for ln in f.readlines():
        s = ln.split()
        try:
            data.append( (
                [int(s[0][i:i+64],2) for i in range(0,256,64)],
                int(s[1],2)
            ) )
        except:
            print (s)
        #if random.random() > 0.999:
        #    break
    print('Shuffling Data...')
    random.shuffle(data)



print('Done Loading Data!')

# Parameters
learning_rate = 0.01
training_epochs = 101
batch_size = 128
display_step = 1

# tf Graph Input
x = tf.placeholder(tf.float32, [None, 8, 8, 4])
y = tf.placeholder(tf.float32, [None, 8, 8, 1])

# Set model weights
#first pass filters
F0a = tf.Variable(tf.random_normal([1,8,4,16]))
F0b = tf.Variable(tf.random_normal([8,1,4,16]))
b0 = tf.Variable(tf.random_normal([16,8,8]))

#Level 1 filters
F1a = tf.Variable(tf.random_normal([1,8,16,16]))
F1b = tf.Variable(tf.random_normal([8,1,16,16]))
b1 = tf.Variable(tf.random_normal([16,8,8]))

#Level 2 filters
F2a = tf.Variable(tf.random_normal([1,8,16,16]))
F2b = tf.Variable(tf.random_normal([8,1,16,16]))
b2 = tf.Variable(tf.random_normal([16,8,8]))

#Level 3 filters
F3a = tf.Variable(tf.random_normal([1,8,16,16]))
F3b = tf.Variable(tf.random_normal([8,1,16,16]))
b3 = tf.Variable(tf.random_normal([16,8,8]))

#Level 3 filters
F4a = tf.Variable(tf.random_normal([1,8,16,16]))
F4b = tf.Variable(tf.random_normal([8,1,16,16]))
b4 = tf.Variable(tf.random_normal([16,8,8]))

#Level 3 filters
F5a = tf.Variable(tf.random_normal([1,8,16,16]))
F5b = tf.Variable(tf.random_normal([8,1,16,16]))
b5 = tf.Variable(tf.random_normal([16,8,8]))

#Level 3 filters
F6a = tf.Variable(tf.random_normal([1,8,16,16]))
F6b = tf.Variable(tf.random_normal([8,1,16,16]))
b6 = tf.Variable(tf.random_normal([16,8,8]))

#Level 3 filters
F7a = tf.Variable(tf.random_normal([1,8,16,16]))
F7b = tf.Variable(tf.random_normal([8,1,16,16]))
b7 = tf.Variable(tf.random_normal([16,8,8]))

#Level 3 filters
F8a = tf.Variable(tf.random_normal([1,8,16,16]))
F8b = tf.Variable(tf.random_normal([8,1,16,16]))
b8 = tf.Variable(tf.random_normal([16,8,8]))

#Consolidation filters
F = tf.Variable(tf.random_normal([1,1,16,1]))
b = tf.Variable(tf.random_normal([8,8,1]))


# Construct model
L0a = tf.transpose(tf.nn.conv2d(x, F0a, [1,1,1,1], 'VALID'),[0,3,1,2])
L0b = tf.transpose(tf.nn.conv2d(x, F0b, [1,1,1,1], 'VALID'),[0,3,1,2])
L0= tf.transpose(tf.tanh(tf.matmul(L0a, L0b) + b0), [0,2,3,1])

L1a = tf.transpose(tf.nn.conv2d(L0, F1a, [1,1,1,1], 'VALID'),[0,3,1,2])
L1b = tf.transpose(tf.nn.conv2d(L0, F1b, [1,1,1,1], 'VALID'),[0,3,1,2])
L1= tf.transpose(tf.tanh(tf.matmul(L1a, L1b) + b1), [0,2,3,1])

L2a = tf.transpose(tf.nn.conv2d(L1, F2a, [1,1,1,1], 'VALID'),[0,3,1,2])
L2b = tf.transpose(tf.nn.conv2d(L1, F2b, [1,1,1,1], 'VALID'),[0,3,1,2])
L2= tf.transpose(tf.tanh(tf.matmul(L2a, L2b) + b2), [0,2,3,1])

L3a = tf.transpose(tf.nn.conv2d(L2, F3a, [1,1,1,1], 'VALID'),[0,3,1,2])
L3b = tf.transpose(tf.nn.conv2d(L2, F3b, [1,1,1,1], 'VALID'),[0,3,1,2])
L3= tf.transpose(tf.tanh(tf.matmul(L3a, L3b) + b3), [0,2,3,1])

L4a = tf.transpose(tf.nn.conv2d(L3, F4a, [1,1,1,1], 'VALID'),[0,3,1,2])
L4b = tf.transpose(tf.nn.conv2d(L3, F4b, [1,1,1,1], 'VALID'),[0,3,1,2])
L4= tf.transpose(tf.tanh(tf.matmul(L4a, L4b) + b4), [0,2,3,1])

L5a = tf.transpose(tf.nn.conv2d(L4, F5a, [1,1,1,1], 'VALID'),[0,3,1,2])
L5b = tf.transpose(tf.nn.conv2d(L4, F5b, [1,1,1,1], 'VALID'),[0,3,1,2])
L5= tf.transpose(tf.tanh(tf.matmul(L5a, L5b) + b5), [0,2,3,1])

L6a = tf.transpose(tf.nn.conv2d(L5, F6a, [1,1,1,1], 'VALID'),[0,3,1,2])
L6b = tf.transpose(tf.nn.conv2d(L5, F6b, [1,1,1,1], 'VALID'),[0,3,1,2])
L6= tf.transpose(tf.tanh(tf.matmul(L6a, L6b) + b6), [0,2,3,1])

L7a = tf.transpose(tf.nn.conv2d(L6, F7a, [1,1,1,1], 'VALID'),[0,3,1,2])
L7b = tf.transpose(tf.nn.conv2d(L6, F7b, [1,1,1,1], 'VALID'),[0,3,1,2])
L7= tf.transpose(tf.tanh(tf.matmul(L7a, L7b) + b7), [0,2,3,1])

L8a = tf.transpose(tf.nn.conv2d(L7, F8a, [1,1,1,1], 'VALID'),[0,3,1,2])
L8b = tf.transpose(tf.nn.conv2d(L7, F8b, [1,1,1,1], 'VALID'),[0,3,1,2])
L8= tf.transpose(tf.tanh(tf.matmul(L8a, L8b) + b8), [0,2,3,1])

pred = tf.nn.sigmoid(tf.nn.conv2d(L8, F, [1,1,1,1], 'VALID') + b)

# Minimize error using cross entropy
cost = tf.reduce_mean(-tf.reduce_sum(y*tf.log(pred), axis=[1,2]))
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
        batch_xs = []
        batch_ys = []
        for _ in range(batch_size):
            itmi = [[[0 for ___ in range(4)] for __ in range(8)] for _ in range(8)]
            itmj = [[[0] for __ in range(8)] for _ in range(8)]
            batch_xs.append(itmi)
            batch_ys.append(itmj)
        for i in range(total_batch):
            ix = i * batch_size

            bd = data[ix:ix+batch_size]
            for k in range(batch_size):
                for Y in range(8):
                    for X in range(8):
                        msk = (1 << (X + 8*Y))
                        if not (bd[k][1] & msk) == 0:
                            itmj[Y][X][0] = 1.0
                        else:
                            itmj[Y][X][0] = 0.0

                        for j in range(4):
                            if not (bd[k][0][j] & msk) == 0:
                                itmi[Y][X][j] = 1.0
                            else:
                                itmi[Y][X][j] = 0.0
                batch_xs.append(itmi)
                batch_ys.append(itmj)

            # Run optimization op (backprop) and cost op (to get loss value)
            _, c = sess.run([optimizer, cost], feed_dict={x: batch_xs,
                                                          y: batch_ys})
            # Compute average loss
            del batch_xs[:]
            del batch_ys[:]
            avg_cost += c
            #print("cost=",c," avg=",avg_cost/(i+1))
            if (i % 100 == 0):
                print(100.0 * i/float(total_batch), '%')
        # Display logs per epoch step
        if (epoch+1) % display_step == 0:
            print("Epoch:", '%04d' % (epoch+1), "cost=", "{:.9f}".format(avg_cost/total_batch))
        saver.save(sess, 'policy_net', global_step=epoch)
        learning_rate = learning_rate * 0.97

    print("Optimization Finished!")

    # Test model
    correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(y, 1))
    # Calculate accuracy
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
