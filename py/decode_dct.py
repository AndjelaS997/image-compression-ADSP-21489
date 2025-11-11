import sys
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

ZZ = np.array([
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
], dtype=np.int32)

def build_C():
    N = 8
    C = np.zeros((8, 8), dtype=np.float32)
    for u in range(8):
        a = np.sqrt(1.0 / N) if u == 0 else np.sqrt(2.0 / N)
        for x in range(8):
            C[u, x] = a * np.cos((np.pi / N) * (x + 0.5) * u)
    return C

C = build_C()

def idct8x8(F):
    return (C.T @ F @ C)

def rgb_to_gray_like_c(rgb_img):
    arr = np.array(rgb_img, dtype=np.uint16)
    R = arr[..., 0]
    G = arr[..., 1]
    B = arr[..., 2]
    Y = (77*R + 150*G + 29*B + 128) >> 8
    return Y.astype(np.uint8)

def mse(a, b):
    a = a.astype(np.float32)
    b = b.astype(np.float32)
    return float(np.mean((a - b) ** 2))

def tokens_from_file(path):
    with open(path, "r", encoding="ascii", errors="ignore") as f:
        for tok in f.read().split():
            yield tok

def decode_dct(path_dct):
    it = tokens_from_file(path_dct)

    magic = next(it)
    if magic != "DCT8_TXT":
        raise ValueError(f"Bad magic: {magic}")

    w = int(next(it)); h = int(next(it)); bxN = int(next(it)); byN = int(next(it))

    qtag = next(it)
    if qtag != "Q":
        raise ValueError("Missing Q line")

    Qvals = [int(next(it)) for _ in range(64)]
    Q = np.array(Qvals, dtype=np.float32).reshape(8, 8)

    img = np.zeros((h, w), dtype=np.float32)

    prev_dc = 0

    for by in range(byN):
        for bx in range(bxN):
            dc_diff = int(next(it))
            dc = prev_dc + dc_diff
            prev_dc = dc

            zz = np.zeros(64, dtype=np.int16)
            zz[0] = np.int16(dc)

            k = 1
            while k < 64:
                run = int(next(it))
                val = int(next(it))
                if run == 0 and val == 0:
                    break
                k += run
                if k >= 64:
                    break
                zz[k] = np.int16(val)
                k += 1

            q = np.zeros(64, dtype=np.int16)
            for i in range(64):
                q[ZZ[i]] = zz[i]

            F = q.astype(np.float32).reshape(8, 8) * Q
            block = idct8x8(F) + 128.0

            y0 = by * 8
            x0 = bx * 8
            for yy in range(8):
                sy = y0 + yy
                if sy >= h: sy = h - 1
                for xx in range(8):
                    sx = x0 + xx
                    if sx >= w: sx = w - 1
                    img[sy, sx] = block[yy, xx]

    return np.clip(img, 0, 255).astype(np.uint8)
    
def show_results(orig_gray, rec, mse_val):
    err = (orig_gray.astype(np.float32) - rec.astype(np.float32)) ** 2

    plt.figure(figsize=(12,4))

    plt.subplot(1,3,1)
    plt.imshow(orig_gray, cmap="gray", vmin=0, vmax=255)
    plt.title("Original (grayscale)")
    plt.axis("off")

    plt.subplot(1,3,2)
    plt.imshow(rec, cmap="gray", vmin=0, vmax=255)
    plt.title(f"Reconstructed\nMSE={mse_val:.2f}")
    plt.axis("off")

    plt.subplot(1,3,3)
    plt.imshow(err, cmap="hot")
    plt.title("Squared error")
    plt.axis("off")
    plt.colorbar(fraction=0.046, pad=0.04)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    dct_path = sys.argv[1] if len(sys.argv) > 1 else "img1.dct"
    bmp_path = sys.argv[2] if len(sys.argv) > 2 else "img1.bmp"

    rec = decode_dct(dct_path)
    Image.fromarray(rec, mode="L").save("reconstructed.png")
    print("Saved reconstructed.png")

    orig_rgb = Image.open(bmp_path).convert("RGB")
    orig_gray = rgb_to_gray_like_c(orig_rgb)
    
    m = mse(orig_gray, rec)
    print("MSE =", m)
    
    show_results(orig_gray, rec, m)
