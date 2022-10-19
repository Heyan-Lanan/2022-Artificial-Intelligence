import numpy as np
import cvxopt


class SupportVectorMachine:
    def __init__(self, C=1, kernel='Linear', epsilon=1e-4):
        self.C = C
        self.epsilon = epsilon
        self.kernel = kernel
        self.sv_alpha = None
        self.positive_num = None
        self.b = None
        self.train_data = None
        self.train_label = None

        # Hint: 你可以在训练后保存这些参数用于预测
        # SV即Support Vector，表示支持向量，SV_alpha为优化问题解出的alpha值，
        # SV_label表示支持向量样本的标签。
        # self.SV = None
        # self.SV_alpha = None
        # self.SV_label = None

    def kERNEL(self, x1, x2, d=2, sigma=1):
        # d for Poly, sigma for Gauss
        if self.kernel == 'Gauss':
            k = np.exp(-(np.sum((x1 - x2) ** 2)) / (2 * sigma ** 2))
        elif self.kernel == 'Linear':
            k = np.dot(x1, x2)
        elif self.kernel == 'Poly':
            k = (np.dot(x1, x2) + 1) ** d
        else:
            raise NotImplementedError()
        return k

    def fit(self, train_data, train_label):
        '''
        TODO：实现软间隔SVM训练算法
        train_data：训练数据，是(N, 7)的numpy二维数组，每一行为一个样本
        train_label：训练数据标签，是(N,)的numpy数组，和train_data按行对应
        '''

        self.train_data = train_data
        self.train_label = train_label
        num = train_data.shape[0]
        q = -1 * np.ones((num, 1))
        q = q.astype(np.double)
        q_cvx = cvxopt.matrix(q)

        a = train_label.reshape(1, num)
        a = a.astype(np.double)
        a_cvx = cvxopt.matrix(a)

        k = np.zeros((num, num))
        for i in range(num):
            for j in range(num):
                k[i][j] = self.kERNEL(train_data[i], train_data[j]) * train_label[i] * train_label[j]
        k = k.astype(np.double)
        k_cvx = cvxopt.matrix(k)

        h = np.zeros((num, 1))
        for i in range(num):
            h[i] = self.C
        h = h.astype(np.double)
        h_cvx = cvxopt.matrix(h)

        b = np.zeros(1)
        b = b.astype(np.double)
        b_cvx = cvxopt.matrix(b)

        ig = np.eye(num, dtype=int)
        ig = ig.astype(np.double)
        ig_cvx = cvxopt.matrix(ig)

        x1 = cvxopt.solvers.qp(k_cvx, q_cvx, ig_cvx, h_cvx, a_cvx, b_cvx)
        self.sv_alpha = np.array(x1['x'])
        self.positive_num = np.where(self.sv_alpha > self.epsilon)[0]
        sum_b = 0
        for j in self.positive_num:
            sum_tmp = 0
            for i in range(num):
                sum_tmp += train_label[i] * self.sv_alpha[i] * self.kERNEL(train_data[i], train_data[j])
            sum_tmp += train_label[j] - sum_tmp
        self.b = sum_b / self.positive_num.shape[0]

    def predict(self, test_data):
        '''
        TODO：实现软间隔SVM预测算法
        train_data：测试数据，是(M, 7)的numpy二维数组，每一行为一个样本
        必须返回一个(M,)的numpy数组，对应每个输入预测的标签，取值为1或-1表示正负例
        '''
        num = test_data.shape[0]
        preds = []
        for j in range(num):
            sum_tmp = 0
            for i in self.positive_num:
                sum_tmp += self.train_label[i] * self.sv_alpha[i] * self.kERNEL(test_data[j], self.train_data[i])
            pred = self.b + sum_tmp
            if pred <= 0:
                preds.append(-1)
            else:
                preds.append(1)
        result = np.array(preds).reshape(num)
        return result
