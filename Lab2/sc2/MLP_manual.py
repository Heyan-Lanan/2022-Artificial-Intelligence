import math

import torch
import numpy as np
from matplotlib import pyplot as plt
import torch.nn as nn

import chart_studio
import chart_studio.plotly as py
import plotly.graph_objects as go

chart_studio.tools.set_credentials_file(
    username='ningyu',
    api_key='GFRRsIuXgMH0teXjCScZ'
)


def act_func(z):
    return np.tanh(z)


def loss_func(outputs):
    labels_zero = np.zeros((1, labels.shape[0]))
    labels_inv = np.argmax(labels, axis=1)
    # print(labels_inv)
    for i in range(labels.shape[0]):
        if outputs[labels_inv[i], i] <= 0:
            pass
            # print(outputs[labels_inv[i], i])
        else:
            labels_zero[0, i] = math.log(outputs[labels_inv[i], i])
    return -1 * np.sum(labels_zero) / labels.shape[0]


class MLP:
    def __init__(self):
        # layer size = [10, 8, 8, 4]
        # 初始化所需参数
        self.num_1 = 10
        self.num_2 = 10
        self.num_3 = 8
        self.num_4 = 8
        self.num_5 = 4
        self.w1 = np.random.rand(10, 10)
        self.w2 = np.random.rand(8, 10)
        self.w3 = np.random.rand(8, 8)
        self.w4 = np.random.rand(4, 8)
        self.b1 = np.random.rand(10)
        self.b2 = np.random.rand(8)
        self.b3 = np.random.rand(8)
        self.b4 = np.random.rand(4)
        self.lr = 0.05
        pass

    def forward(self, x):
        # 前向传播

        h1_out = np.dot(self.w1, x) + np.dot(np.diag(self.b1), np.ones((self.num_2, 100)))
        h1_out = act_func(h1_out)
        h2_out = np.dot(self.w2, h1_out) + np.dot(np.diag(self.b2), np.ones((self.num_3, 100)))
        h2_out = act_func(h2_out)
        h3_out = np.dot(self.w3, h2_out) + np.dot(np.diag(self.b3), np.ones((self.num_4, 100)))
        h3_out = act_func(h3_out)
        h4_out = np.dot(self.w4, h3_out) + np.dot(np.diag(self.b4), np.ones((self.num_5, 100)))

        for i in range(h4_out.shape[0]):
            for j in range(h4_out.shape[1]):
                h4_out[i, j] = np.exp(h4_out[i, j])

        out_col = np.sum(h4_out, axis=0)

        for i in range(h4_out.shape[0]):
            for j in range(h4_out.shape[1]):
                h4_out[i][j] = h4_out[i][j] / out_col[j]

        loss = loss_func(h4_out)
        return h4_out, h3_out, h2_out, h1_out, loss
        pass

    def backward(self, a4, a3, a2, a1, x):  # 自行确定参数表
        # 反向传播
        ones = np.ones((100, 1))

        l_z4 = a4 - labels.T
        l_w4 = np.dot(l_z4, a3.T)
        l_b4 = np.dot(l_z4, ones)

        # print(self.w3.T.shape)
        # print(l_z4.shape)

        l_z3 = np.multiply(np.dot(self.w4.T, l_z4), np.ones((a3.shape[0], a3.shape[1])) - np.multiply(a3, a3))
        l_w3 = np.dot(l_z3, a2.T)
        l_b3 = np.dot(l_z3, ones)

        l_z2 = np.multiply(np.dot(self.w3.T, l_z3), np.ones((a2.shape[0], a2.shape[1])) - np.multiply(a2, a2))
        l_w2 = np.dot(l_z2, a1.T)
        l_b2 = np.dot(l_z2, ones)

        l_z1 = np.multiply(np.dot(self.w2.T, l_z2), np.ones((a1.shape[0], a1.shape[1])) - np.multiply(a1, a1))
        l_w1 = np.dot(l_z1, x)
        l_b1 = np.dot(l_z1, ones)

        self.w1 -= self.lr * l_w1
        self.w2 -= self.lr * l_w2
        self.w3 -= self.lr * l_w3
        self.w4 -= self.lr * l_w4
        # print(l_b1)
        # print(self.b1)
        self.b1 -= self.lr * l_b1.reshape(self.num_2)
        self.b2 -= self.lr * l_b2.reshape(self.num_3)
        self.b3 -= self.lr * l_b3.reshape(self.num_4)
        self.b4 -= self.lr * l_b4.reshape(self.num_5)

        pass


class MLP_2(nn.Module):
    def __init__(self):
        super(MLP_2, self).__init__()
        self.l1 = nn.Linear(10, 10, bias=True)
        self.l2 = nn.Linear(10, 8, bias=True)
        self.l3 = nn.Linear(8, 8, bias=True)
        self.l4 = nn.Linear(8, 4, bias=True)

    def forward(self, x):
        h1 = torch.tanh(self.l1(x))
        h2 = torch.tanh(self.l2(h1))
        h3 = torch.tanh(self.l3(h2))
        out = torch.softmax(self.l4(h3), dim=1)
        return out


def train():
    """
        mlp: 传入实例化的MLP模型
        epochs: 训练轮数
        lr: 学习率
        inputs: 生成的随机数据
        labels: 生成的one-hot标签
    """
    loss_list = []
    for i in range(epochs):
        h4_out, h3_out, h2_out, h1_out, loss = mlp.forward(inputs.T)
        loss_list.append(loss)
        mlp.backward(h4_out, h3_out, h2_out, h1_out, inputs)
    print("W1: ")
    print(mlp.w1)
    print("W2: ")
    print(mlp.w2)
    print("W3: ")
    print(mlp.w3)
    print("W4: ")
    print(mlp.w4)
    print("b1: ")
    print(mlp.b1)
    print("b2: ")
    print(mlp.b2)
    print("b3: ")
    print(mlp.b3)
    print("b4: ")
    print(mlp.b4)

    return loss_list


def train_2(net: MLP_2, inputs_1, labels_1, epochs_1, optimizer_1):
    loss_2 = []
    net.double()
    for i in range(epochs_1):
        optimizer_1.zero_grad()
        out = net.forward(inputs_1)
        temp = torch.log(torch.sum(torch.mul(out, labels_1), axis=1))
        loss = -temp.mean()
        loss.backward()
        optimizer_1.step()
        loss_2.append(loss.item())
    return loss_2


if __name__ == '__main__':
    # 设置随机种子,保证结果的可复现性
    np.random.seed(1)
    mlp = MLP()
    mlp_2 = MLP_2()
    # 生成数据
    inputs = np.random.randn(100, 10)

    # 生成one-hot标签
    labels = np.eye(4)[np.random.randint(0, 4, size=(1, 100))].reshape(100, 4)

    # 训练
    epochs = 1000
    lr = 0.1
    loss_list = train()
    optimizer = torch.optim.SGD(mlp_2.parameters(), lr=lr)
    loss_list_2 = train_2(mlp_2, torch.tensor(inputs), torch.tensor(labels), epochs, optimizer)

    # plt.figure(figsize=(12, 5))
    # plt.subplot(121)
    plt.plot(loss_list)
    plt.plot(loss_list_2, 'r')
    plt.xlabel("epoch")
    plt.ylabel("loss")
    plt.legend(['mlp_loss', 'pytorch_loss'])
    plt.show()

    epochs_list = np.zeros(epochs)
    for i in range(epochs):
        epochs_list[i] = i
    trace1 = go.Scatter(
        x=epochs_list,
        y=loss_list,
        mode='lines',  # 线的绘图
        name='MLP'
    )
    trace2 = go.Scatter(
        x=epochs_list,
        y=loss_list_2,
        mode='lines',  # 线的绘图
        name='pytorch'
    )
    data = [trace1, trace2]
    layout = go.Layout(
        title='损失函数与迭代次数关系折线图',
        xaxis=dict(title="epoch"),
        yaxis=dict(title="loss"),
    )

    # fig = go.Figure(data=data, layout=layout)
    # py.plot(fig, filename='MLP')
