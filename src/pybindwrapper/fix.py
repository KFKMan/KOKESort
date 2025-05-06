import zipfile
import os
import shutil

WHL_PATH = "dist/KOKESortWrapper-0.1.0-cp313-cp313-win_amd64.whl"
TEMP_DIR = "unpacked_whl"
OUTPUT_WHL = WHL_PATH

# 1. Eski dizini sil
if os.path.exists(TEMP_DIR):
    shutil.rmtree(TEMP_DIR)

# 2. .whl dosyasını aç ve çıkar
with zipfile.ZipFile(WHL_PATH, 'r') as zip_ref:
    zip_ref.extractall(TEMP_DIR)

# 3. Release veya Debug klasörünü bul
for build_dir in ['Release', 'Debug']:
    full_path = os.path.join(TEMP_DIR, build_dir)
    if os.path.isdir(full_path):
        target_dir = os.path.join(TEMP_DIR, 'KOKESortWrapper')
        os.makedirs(target_dir, exist_ok=True)

        for file in os.listdir(full_path):
            shutil.move(os.path.join(full_path, file), target_dir)

        shutil.rmtree(full_path)
        break

# 4. Eski .whl dosyasını sil
os.remove(WHL_PATH)

# 5. Yeni .whl dosyasını oluştur
with zipfile.ZipFile(OUTPUT_WHL, 'w', zipfile.ZIP_DEFLATED) as zipf:
    for root, _, files in os.walk(TEMP_DIR):
        for file in files:
            full_path = os.path.join(root, file)
            rel_path = os.path.relpath(full_path, TEMP_DIR)
            zipf.write(full_path, rel_path)

# 6. Temizle
shutil.rmtree(TEMP_DIR)

print("Fixed .whl successfully!")