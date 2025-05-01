#!/usr/bin/env python3
import subprocess
import sys
import io

sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

def main():
    print("Using Mutate Wrapper")

    if len(sys.argv) < 2:
        print("Usage: mutant_wrapper.py <path-to-mutant-binary> [args...]")
        sys.exit(1)

    cmd = sys.argv[1:]

    print("Running command:", cmd)
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=10  # İstersen zaman aşımı koyabilirsin
        )
    except subprocess.SubprocessError as e:
        print(f"ERROR: subprocess failed to run: {e}")
        sys.exit(0)  # Fail to run → crashed → mutant öldü

    stdout = result.stdout
    stderr = result.stderr
    output = stdout + stderr

    # Log çıktıyı
    print(output)

    # Çökme belirtileri varsa mutant öldü → test başarılı
    if ("Segmentation fault" in output or
        "Aborted" in output or
        "Assertion" in output or
        result.returncode != 0):
        print("✅ Mutant killed (test passed)")
        sys.exit(0)
    else:
        print("❌ Mutant survived (test failed)")
        sys.exit(1)

if __name__ == "__main__":
    main()