<!-- -*- coding: utf-8 -*- -->
# noconsole --- コンソールウィンドウなしでコンソールアプリを実行する ---

コンソールウィンドウ（コマンドプロンプト）を出さずに
コンソールアプリを実行します。

## 必要なもの

ビルドには以下が必要です。

- Mingw-w64 gcc

等

## ビルド

64 bit 版は make 一発でビルドできます。
32 bit 版は Makefile の CC = の行を取り換えてください。

## 使い方

例： cygwin emacs をコンソールウィンドウなしで起動する

noconsole c:\cygwin64\bin\env.exe /usr/bin/emacs

これをショートカットにして、デスクトップに置くなり、
タスクバーに置くなりすると cygwin emacs の起動が便利になります。
アイコンも emacs のものに変えて mingw emacs 付属の runemacs.exe
的な使い方ができるようになります。

## ライセンス

Copyright (C) 2016 Masamichi Hosoda. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
