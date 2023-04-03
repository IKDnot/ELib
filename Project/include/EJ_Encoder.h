/**
 * @file           EJ_Encoder.h
 * @brief          2相エンコーダを制御するクラスEJ_Encoderクラスと、EJ_Encoderクラスを管理するEJ_Encoder_Managerクラスの定義
 * @author         IKDnot
 * @date           2023/03/30
 * 
 * License
 * 
 * Copyright (c) 2023 IKDnot
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EJENCODER
#define EJENCODER
#include <Arduino.h>

/**
 * @struct EncoderDef
 * @brief 1つのエンコーダを定義する構造体
 */
typedef struct
{
    uint8_t a; /**< A相接続ピン */
    uint8_t b; /**< B相接続ピン */
    uint8_t id;   /**< モーターの識別番号 */
} EncoderDef;

/**
 * @brief エンコーダーを制御するクラス
 * @details *注意:本クラスのインスタンスはEJ_Encoder_Managerクラス以外からは生成できない
 */
class EJ_Encoder
{
protected:
    /**
     * @brief EJ_Encoderクラスのコンストラクタ
     * @param pin1 A相接続ピン
     * @param pin2 B相接続ピン
     */
    EJ_Encoder(uint8_t a = 0, uint8_t b = 0);

    friend class EJ_Encoder_Manager;

public:
    /**
     * @brief EJ_Encoder クラスのデストラクタ
     */
    ~EJ_Encoder();

    /**
     * @brief モータの回転数を取得する
     */
    long long getCount();

    /**
     * @brief モータの角度を取得する
     */
    float getAngle();

    /** 
     * @brief モータの回転数をリセットする
    */
    void reset();

    /**
     * @brief ピン割り込み時にコールされる関数
     */
    void onInterrupt();

private:
    static const char* _classname;
    uint8_t _a;
    uint8_t _b;
    long long _count;
    int _lastEncoded;
};

/**
 * @brief EJ_Encoderクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class EJ_Encoder_Manager
{
private:
    /**
     * @brief EJ_Encoder_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_Encoderの最大インスタンス数の定義
     */
    EJ_Encoder_Manager(size_t maxInstanceSize);

private:
    /**
     * @brief EJ_Encoder_Manager のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return EJ_Encoder_Managerのsingletonを指すポインタ
     */
    static EJ_Encoder_Manager* getInstance();

public:
    /**
     * @brief EJ_Encoder_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_Encoderクラスのインスタンスをすべて解放する
     */
    ~EJ_Encoder_Manager();

public:
    /**
     * @brief EJ_Encoder_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_Encoderの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_Encoderクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetEncoder関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param EncoderDef 参照
     * @return EJ_Encoderクラスのインスタンスを指すポインタ
     */
    static EJ_Encoder *createEncoder(EncoderDef Encoder);

    /**
     * @brief EJ_Encoderクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetEncoder関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param pin1 A相接続ピン
     * @param pin2 A相接続ピン
     * @param id モーターの識別番号
     * @return EJ_Encoderクラスのインスタンスを指すポインタ
     */
    static EJ_Encoder *createEncoder(uint8_t a, uint8_t b, uint8_t id);

    /**
     * @brief EJ_Encoderクラスのインスタンスを取得する
     * @details createEncoder関数で生成したEJ_Encoderクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param EncoderDef 取得したいエンコーダを指すエンコーダ定義構造体
     * @return EJ_Encoderクラスのインスタンスを指すポインタ
     */
    static EJ_Encoder *getEncoder(EncoderDef Encoder);

    /**
     * @brief EJ_Encoderクラスのインスタンスを取得する
     * @details createEncoder関数で生成したEJ_Encoderクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id モーターの識別番号
     * @return EJ_Encoderクラスのインスタンスを指すポインタ
     */
    static EJ_Encoder *getEncoder(uint8_t id);

private:
    static const char* _classname;
    static EJ_Encoder_Manager *_singleton;
    const size_t _maxInstanceSize;
    EJ_Encoder **_instanceList;
};

#endif // EJENCODER