import argparse

def generate_sequence(start, end, step, mode):
    sequence = []
    curr = start
    
    while curr <= end:
        sequence.append(f'{curr:.0f}')
        
        if mode == "+":
            curr += step
        elif mode == "*":
            curr *= step
        elif mode == "/":
            if step == 0:
                raise ValueError("Step value cannot be zero for division mode.")
            curr /= step
            if curr < start:
                break
        else:
            raise ValueError("Invalid mode. Use '+', '*' or '/'.")
    
    return ",".join(sequence)

def main():
    parser = argparse.ArgumentParser(description="Generate and save unsorted arrays with different step modes.")
    parser.add_argument("--start", type=int, default=1000, help="Starting value")
    parser.add_argument("--end", type=int, default=10000, help="End value")
    parser.add_argument("--step", type=float, default=100, help="Step value")
    parser.add_argument("--mode", type=str, choices=["+", "*", "/"], default="+", help="Step mode: '+' for addition, '*' for multiplication, '/' for division")
    
    args = parser.parse_args()
    
    result = generate_sequence(args.start, args.end, args.step, args.mode)
    print(result)

if __name__ == "__main__":
    main()