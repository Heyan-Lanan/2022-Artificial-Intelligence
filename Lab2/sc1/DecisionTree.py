import csv

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from graphviz import Digraph


def info_func(data):
    info = 0
    labels = data.iloc[:, -1]
    label_count = labels.value_counts()
    label_num = len(labels)
    for k in label_count.keys():
        temp = label_count[k] / label_num
        info += -temp * np.log(temp)
    return info


def info_gain(data, feature):
    info = info_func(data)
    feature_count = data[feature].value_counts()
    info_2 = 0
    data_num = data.shape[0]
    for key in feature_count.keys():
        key_w = feature_count[key] / data_num
        data_loc = data.loc[data[feature] == key]
        info_key = info_func(data_loc)
        info_2 += key_w * info_key
    gain = info - info_2
    return gain


class DecisionTree:

    def __init__(self):
        self.tree = None
        pass

    def fit(self, train_features, train_labels):
        '''
        TODO: 实现决策树学习算法.
        train_features是维度为(训练样本数,属性数)的numpy数组
        train_labels是维度为(训练样本数, )的numpy数组
        '''
        f = open('test.csv', 'w', encoding='utf-8', newline='')
        csv_writer = csv.writer(f)
        csv_writer.writerow(
            ["label1", "label2", "label3", "label4", "label5", "label6", "label7", "label8", "label9", "value"])
        for i in range(train_features.shape[0]):
            temp = list(train_features[i])
            temp.append(train_labels[i])
            csv_writer.writerow(temp)
        f.close()
        data = pd.read_csv('test.csv', engine='python')
        self.tree = set_dic_tree(data)
        print(self.tree)
        # print(self.tree["label6"].keys())
        # print(self.tree["label6"][1]["label3"].keys())
        # print(self.tree["label6"][2].keys())
        # print(self.tree["label6"][0].keys())

        # plot_model(self.tree["label6"][0], "s_tree0.gv")
        # plot_model(self.tree["label6"][1], "self_tree1.gv")
        # plot_model(self.tree["label6"][2], "self_tree2.gv")

        # with open('example.csv', 'w') as fp:
        #     fp.write('''label1,label2,label3,label4,label5,label6,label7,label8,label9,value''')
        #     for item in train_features:
        #         fp.write(item)

    def predict(self, test_features):
        '''
        TODO: 实现决策树预测.
        test_features是维度为(测试样本数,属性数)的numpy数组
        该函数需要返回预测标签，返回一个维度为(测试样本数, )的numpy数组
        '''
        test_num = test_features.shape[0]
        preds = np.zeros(test_num)
        for i in range(test_num):
            test_dic = {}
            for j in range(9):
                key = "label" + str(j + 1)
                test_dic[key] = test_features[i][j]
            # print(test_dic)
            test_pred = prediction(self.tree, test_dic)
            preds[i] = test_pred
        return preds


def sel_label(data):
    labels = data.iloc[:, -1]
    sort_1 = labels.value_counts(sort=True)
    # print(sort_1)
    return sort_1.keys()[0]


def set_feature(data):
    features = data.columns[:-1]
    best = {}
    for fea in features:
        best[fea] = info_gain(data, fea)
    best = sorted(best.items(), key=lambda x: x[1], reverse=True)
    return best[0][0]


def del_feature(data, best):
    temp = pd.unique(data[best])
    data_2 = []
    data_3 = []
    for item in temp:
        data_2.append((item, data[data[best] == item]))
    # print(data_2)
    for item in data_2:
        data_3.append((item[0], item[1].drop([best], axis=1)))
    return data_3


def set_dic_tree(data_list):
    fea_list = []
    for fea in data_list.iloc[:, :-1].columns:
        val_list = list(pd.unique(data_list[fea]))
        fea_list.append((fea, val_list))
    fea_dic = dict(fea_list)
    labels = data_list.iloc[:, -1]
    # print(labels)
    # for i in data.iloc[:, :-1].columns:
    #     print(data[i].value_counts())
    labels_count = len(labels.value_counts())
    if labels_count == 1:
        # print(labels.values[0])
        return labels.values[0]

    sign = 1
    for item in data_list.iloc[:, :-1].columns:
        if len(data_list[item].value_counts()) != 1:
            sign = 0
    if sign:
        # print("1111111")
        return sel_label(data_list)
    best = set_feature(data_list)
    dic_tree = {best: {}}
    values = pd.unique(data_list[best])

    # print(best)
    # print(values)

    if len(values) != len(fea_dic[best]):
        # print(best)
        # print(data)
        other_vals = set(fea_dic[best]) - set(values)
        for val in other_vals:
            dic_tree[best][val] = sel_label(data_list)

    for item in del_feature(data_list, best):
        dic_tree[best][item[0]] = set_dic_tree(item[1])
    return dic_tree


def prediction(dic_tree, test_data):
    f_1 = list(dic_tree.keys())[0]
    dic_2 = dic_tree[f_1]
    in_1 = test_data[f_1]
    if in_1 in dic_2.keys():
        in_value = dic_2[in_1]
    else:
        # print(in_1)
        return 0
    if isinstance(in_value, dict):
        return prediction(in_value, test_data)
    else:
        return in_value


def getMaxLeafs(mydic_tree):
    numLeaf = len(mydic_tree.keys())
    for key, value in mydic_tree.items():
        if isinstance(value, dict):
            sum_numLeaf = getMaxLeafs(value)
            if sum_numLeaf > numLeaf:
                numLeaf = sum_numLeaf
    return numLeaf


def plot_model(tree, name):
    g = Digraph("G", filename=name, format='png', strict=False)
    first_label = list(tree.keys())[0]
    g.node("0", first_label)
    _sub_plot(g, tree, "0")
    leafs = str(getMaxLeafs(tree) // 10)
    g.attr(rankdir='LR', ranksep=leafs)

    g.view()


root = "0"


def _sub_plot(g, tree, inc):
    global root

    first_label = list(tree.keys())[0]
    ts = tree[first_label]
    for i in ts.keys():
        if isinstance(tree[first_label][i], dict):
            root = str(int(root) + 1)
            g.node(root, list(tree[first_label][i].keys())[0])
            g.edge(inc, root, str(i))
            _sub_plot(g, tree[first_label][i], root)
        else:
            root = str(int(root) + 1)
            g.node(root, str(tree[first_label][i]))
            g.edge(inc, root, str(i))
