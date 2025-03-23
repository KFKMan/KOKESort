import os
import argparse


def main():
    parser = argparse.ArgumentParser(description="Generate and save unsorted arrays.")
    parser.add_argument("--start", type=int, default=1000)
    parser.add_argument("--end", type=int, default=10000)
    parser.add_argument("--step", type=int, default=100)
    
    args = parser.parse_args()

    pr = ""

    curr = args.start;
    while(curr <= args.end):
        pr += str(curr) + ",";
        curr += args.step;
        
    pr = pr[:-1]

    print(pr)

if __name__ == "__main__":
    main()