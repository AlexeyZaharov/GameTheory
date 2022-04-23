import os
os.environ["PATH"] += os.pathsep + 'C:\\Program Files\\Graphviz\\bin'

import numpy
import uuid
from treelib import Tree
import random
from graphviz import Source

DEPTH = 6


def create_subtree(current_depth, nid):
    tree = Tree()
    if (current_depth % 2):
        tree.create_node("A : ", nid)
    else:
        tree.create_node("B : ", nid)

    # current_depth % 2 + 2 == children_num
    if current_depth + 1 == DEPTH - 1 or random.random() > 0.8:
        random_data = [(random.randint(0, 20), random.randint(0, 20)) for i in range(current_depth % 2 + 2)]
        children_probability = random.random()
        tree.create_node(random_data[0], uuid.uuid4(), nid, data=[random_data[0]])
        if children_probability > 0.3:
            tree.create_node(random_data[1], uuid.uuid4(), nid, data=[random_data[1]])
        if children_probability > 0.7 and current_depth % 2 + 2 == 3:
            tree.create_node(random_data[2], uuid.uuid4(), nid, data=[random_data[2]])

        return tree

    tree.paste(nid, create_subtree(current_depth + 1, uuid.uuid4()))
    tree.paste(nid, create_subtree(current_depth + 1, uuid.uuid4()))

    if current_depth % 2 + 2 == 3:
        tree.paste(nid, create_subtree(current_depth + 1, uuid.uuid4()))

    return tree

def level_slice_tree(tree, level):
    return list(filter(lambda x: tree.level(x.identifier) == level, tree.all_nodes()))

def find_best_path(tree: Tree):
    current_depth = tree.depth() - 1
    copy_tree = Tree(tree)

    while (current_depth != -1):
        gamer = current_depth % 2
        for v in level_slice_tree(tree, current_depth):
            if v.is_leaf():
                continue

            children = tree.children(v.identifier)
            max_win = numpy.max(
                [numpy.max([child.data[0][gamer] for i in range(len(child.data[0]))]) for child in children])

            for child in children:
                if child.data[0][gamer] < max_win:
                    tree.remove_subtree(child.identifier)
                else:
                    copy_tree.get_node(v.identifier).tag += f"\n{str([child.data[0]])}"
            v.data = [child.data[0] for child in tree.children(v.identifier)]

        current_depth -= 1

    tree.show()
    copy_tree.to_graphviz("result_tree.gv")
    Source.from_file("result_tree.gv").render(cleanup=True, format='pdf', view=True)
    return tree


def create_tree():
    tree = Tree()
    tree.create_node("A : 0", 0)
    tree.paste(0, create_subtree(1, uuid.uuid4()))
    tree.paste(0, create_subtree(1, uuid.uuid4()))

    current_depth = 0
    id = 1
    while (current_depth != DEPTH):
        for v in level_slice_tree(tree, current_depth):
            children = tree.children(v.identifier)
            for child in children:
                if child.is_leaf():
                    continue
                tree.get_node(child.identifier).tag = ("A : " if current_depth % 2 else "B : ") + str(id)
                id += 1
        current_depth += 1

    return tree


if __name__ == '__main__':
    my_tree = create_tree()
    my_tree.show(key=False, idhidden=False)
    my_tree.to_graphviz("init_tree.gv")
    Source.from_file("init_tree.gv").render(cleanup=True, format='pdf', view=True)

    find_best_path(my_tree)
    filename = "truncated_tree.gv"
    my_tree.to_graphviz(filename)
    Source.from_file("truncated_tree.gv").render(cleanup=True, format='pdf', view=True)
