import random, sys

test_cnt = 0

try:
    test_cnt = int(input("Number of random tests: "))
except:
    print("Wrong Input, Try Again...")
    sys.exit()

test_list = []

class Block:
    def __init__(self):
        self.start_val = random.randint(1,254)
        self.expected_val = self.start_val ^ 0xff
    
    def block_check_pass(self, inp):
        if self.expected_val == inp:
            return "PASS"
        else:
            return "FAIL"

    def block_get_info(self):
        return [self.start_val, self.expected_val]

for _ in range(test_cnt):
    test_list.append(Block())

for item in test_list:
    bv = item.block_get_info()
    print(f"{item.block_check_pass(bv[1])}: Initial Value: {bv[0]:08b} - Expected Value: {bv[1]:08b} - Recived Value: WIP")
