#!/usr/bin/env python

from util import *

lgw = 5
w = 1 << lgw
u = w // 8


def ROL(n, k):
    k %= w
    n %= 1 << w
    return ((n << k) % (1 << w)) | (n >> (w - k))


class RC6(object):
    def __init__(self, K, r=20):
        L = bytes_to_words(K, u, 'little')

        t = 2 * r + 4
        P = (0xb7e151628aed2a6b >> (64 - w)) | 1
        Q = (0x9e3779b97f4a7c15 >> (64 - w)) | 1
        S = [P + Q * i for i in range(t)]

        A, B = 0, 0
        c = len(K) // u
        for k in range(3 * max(t, c)):
            i, j = k % t, k % c
            A = S[i] = ROL(S[i] + A + B, 3)
            B = L[j] = ROL(L[j] + A + B, A + B)

        self.r = r
        self.S = S

    def block(self, X, revert=False):
        X = bytes_to_words(X, u, 'little')
        S = self.S
        A, B, C, D = X

        if revert:
            C -= S[2 * self.r + 3]
            A -= S[2 * self.r + 2]
            for i in reversed(range(self.r)):
                A, B, C, D = D, A, B, C
                v = ROL(D * (2 * D + 1), lgw)
                t = ROL(B * (2 * B + 1), lgw)
                C = ROL(C - S[2 * i + 3], -t) ^ v
                A = ROL(A - S[2 * i + 2], -v) ^ t
            D -= S[1]
            B -= S[0]
        else:
            B += S[0]
            D += S[1]
            for i in range(self.r):
                t = ROL(B * (2 * B + 1), lgw)
                v = ROL(D * (2 * D + 1), lgw)
                A = ROL(A ^ t, v) + S[2 * i + 2]
                C = ROL(C ^ v, t) + S[2 * i + 3]
                A, B, C, D = B, C, D, A
            A += S[2 * self.r + 2]
            C += S[2 * self.r + 3]

        X = words_to_bytes([A, B, C, D], 4, 'little')
        return X


if __name__ == "__main__":

    ROUND = 1000

    key = hexa_to_bytes(
        "0123456789abcdef0112233445566778899aabbccddeeff01032547698badcfe")
    cipher = hexa_to_bytes("c8241816f0d7e48920ad16a1674e5d48")
    user = hexa_to_bytes("02132435465768798a9bacbdcedfe0f1")


    # print("Encrypting: 02132435465768798a9bacbdcedfe0f1")
    # print("--> " + bytes_to_hexa(crypt.block(user, False)))
    # print("Decrypting: c8241816f0d7e48920ad16a1674e5d48")
    # print("--> " + bytes_to_hexa(crypt.block(cipher, True)))
    while True:
        crypt = RC6(key)
        crypt.block(user, False)
        crypt.block(cipher, True)