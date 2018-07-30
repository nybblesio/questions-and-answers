#include <map>
#include <stack>
#include <vector>
#include <iomanip>
#include <iostream>

struct color_node_t {
    size_t x = 0;
    size_t y = 0;
    uint8_t color = 0;
    bool counted = false;
    bool visited = false;
    color_node_t* up = nullptr;
    color_node_t* down = nullptr;
    color_node_t* left = nullptr;
    color_node_t* right = nullptr;
};

using color_node_list_t = std::vector<color_node_t>;
using color_node_result_t = std::map<uint8_t, size_t>;

bool load_color_grid(
        size_t height,
        size_t width,
        color_node_list_t& list,
        const std::string& input) {
    for (const auto& c : input) {
        char temp[2];
        temp[1] = '\0';
        temp[0] = c;
        color_node_t node {};
        node.color = static_cast<uint8_t>(std::atoi(temp));
        list.push_back(node);
    }

    size_t index = 0;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            auto& node = list[index++];
            node.x = x;
            node.y = y;
            if (y > 0) {
                node.up = &list[((y - 1) * width) + x];
            }
            if (y < height - 1) {
                node.down = &list[((y + 1) * width) + x];
            }
            if (x > 0) {
                node.left = &list[(y * width) + (x - 1)];
            }
            if (x < width - 1) {
                node.right = &list[(y * width) + (x + 1)];
            }
        }
    }

    return true;
}

void walk_color_node(
        color_node_t* node,
        color_node_result_t& results) {
    if (node == nullptr)
        return;

    node->visited = true;
    auto& count = results[node->color];

    if (node->up != nullptr) {
        if (!node->up->visited) {
            walk_color_node(node->up, results);
        }
        if (!node->counted && node->up->color == node->color) {
            count++;
            node->counted = true;
        }
    }

    if (node->right != nullptr) {
        if (!node->right->visited) {
            walk_color_node(node->right, results);
        }
        if (!node->counted && node->right->color == node->color) {
            count++;
            node->counted = true;
        }
    }

    if (node->down != nullptr) {
        if (!node->down->visited) {
            walk_color_node(node->down, results);
        }
        if (!node->counted && node->down->color == node->color) {
            count++;
            node->counted = true;
        }
    }

    if (node->left != nullptr) {
        if (!node->left->visited) {
            walk_color_node(node->left, results);
        }
        if (!node->counted && node->left->color == node->color) {
            count++;
            node->counted = true;
        }
    }
}

void format_node(color_node_t* node) {
    node->visited = true;
    std::cout << "node.x: " << node->x << ", node.y: " << node->y << "\n";
    if (node->up != nullptr && !node->up->visited) {
        std::cout << "up\n";
        format_node(node->up);
    }
    if (node->right != nullptr && !node->right->visited) {
        std::cout << "right\n";
        format_node(node->right);
    }
    if (node->down != nullptr && !node->down->visited) {
        std::cout << "down\n";
        format_node(node->down);
    }
    if (node->left != nullptr && !node->left->visited) {
        std::cout << "left\n";
        format_node(node->left);
    }
    std::cout << "\n";
}

int main() {
    const size_t width = 8;
    const size_t height = 8;

    std::string test_grid =
        "00000000"
        "01022200"
        "01002000"
        "01112000"
        "00302000"
        "00302000"
        "00300000"
        "03333333";

    color_node_list_t list {};
    if (!load_color_grid(height, width, list, test_grid))
        std::cout << "unable to load grid" << std::endl;

//    format_node(&list[0]);

    color_node_result_t results {};
    for (size_t i = 0; i < 10; i++)
        results.insert(std::make_pair(i, 0));

#if 1
    auto node = &list[0];
    std::stack<color_node_t*> stack {};

    while (true) {
        auto& count = results[node->color];

        if (node->visited) {
            goto _next;
        }

        node->visited = true;

        if (node->up != nullptr) {
            if (!node->up->visited) {
                stack.push(node->up);
            }
            if (!node->counted && node->up->color == node->color) {
                count++;
                node->counted = true;
            }
        }

        if (node->right != nullptr) {
            if (!node->right->visited) {
                stack.push(node->right);
            }
            if (!node->counted && node->right->color == node->color) {
                count++;
                node->counted = true;
            }
        }

        if (node->down != nullptr) {
            if (!node->down->visited) {
                stack.push(node->down);
            }
            if (!node->counted && node->down->color == node->color) {
                count++;
                node->counted = true;
            }
        }

        if (node->left != nullptr) {
            if (!node->left->visited) {
                stack.push(node->left);
            }
            if (!node->counted && node->left->color == node->color) {
                count++;
                node->counted = true;
            }
        }

    _next:
        if (stack.empty())
            break;
        node = stack.top();
        stack.pop();
    }
#else
    walk_color_node(&list[0], results);
#endif

    std::cout << "color count\n";
    for (const auto& kvp : results) {
        std::cout << (size_t)kvp.first << ": " << kvp.second << "\n";
    }

    std::cout << std::endl;

    return 0;
}