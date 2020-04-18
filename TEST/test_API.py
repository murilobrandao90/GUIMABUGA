import unittest
import Consumidor

class testAPI(unittest.TestCase):

    def test_check(self):
        result = Consumidor.check()
        self.assertEqual(result,200)
    
    def test_add(self):
        result = Consumidor.adicionar()
        self.assertEqual(result,200)

if __name__ == "__main__":
    unittest.main()