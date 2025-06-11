import os
import random, sys, serial, time

class comm:
	def __init__(self):
		self.com = os.open('/dev/smpinv', os.O_RDWR)
		return

	def read(self):
		r = os.read(self.com, 1)
		if r:
			return r[0]
		return None
		#return r.decode('utf-8', errors='ignore')

	def write(self, s_input):
		os.write(self.com, s_input.to_bytes(1, 'little'))

	def close(self):
		os.close(self.com)

class Block:
	def __init__(self):
		self.s_val = random.randint(1,254)
		self.e_val = self.s_val ^ 0xFF

	def block_verify_test(self, inp):
		if self.e_val == inp:
			return "PASS"
		return "FAIL"

	def block_get_info(self):
		return [self.s_val, self.e_val]

def main():
	s_com = comm()
	test_cases = []

	test_cnt = 0

	# Generate Test Cases
	for _ in range(0, 200):
		test_cases.append(Block())

	for item in test_cases:
		b_info = item.block_get_info()
		s_com.write(b_info[0])

		time.sleep(0.1)

		out = None
		while True:
			out = s_com.read()
			if out is not None and out != '':
				break
			time.sleep(0.1)
		
		if out != '' and out is not None:
			print(f"{test_cnt+1}.\t{item.block_verify_test(out)}\tInitial Value: {int(b_info[0]):08b}\tExpected Value: {int(b_info[1]):08b}\tGot:{out:08b}")
		test_cnt += 1

main()
