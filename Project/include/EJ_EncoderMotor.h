/**
 * @file           EJ_EncoderMotor.h
 * @brief          エンコーダ付きモーターを制御するクラスEJ_EncoderMotorクラスと、EJ_EncoderMotorクラスを管理するEJ_EncoderMotor_Managerクラスの定義
 * @author         IKDnot
 * @date           2023/03/13
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

#ifndef EJENCODERMOTOR
#define EJENCODERMOTOR
#include <Arduino.h>
#include <Encoder.h>
#include "EJ_DCMotor.h"

/**
 * @struct EncoderMotorDef
 * @brief 1つのエンコーダ付きモータを定義する構造体
 */
typedef struct
{
    uint8_t pin1; /**< モーターの接続ピン1 */
    uint8_t pin2; /**< モーターの接続ピン2 */
    uint8_t enc1; /**< エンコーダの接続ピン1 */
    uint8_t enc2; /**< エンコーダの接続ピン1 */
    uint8_t id;   /**< モーターの識別番号 */
} EncoderMotorDef;

/**
 * @brief エンコーダ付きモーターを制御するクラス
 * @details *注意:本クラスのインスタンスはEJ_EncoderMotor_Managerクラス以外からは生成できない
 */
class EJ_EncoderMotor : public EJ_DCMotor, public Encoder
{
private:
    /**
     * @brief EJ_EncoderMotorクラスのコンストラクタ
     * @param pin1 モーターの接続ピン1
     * @param pin2 モーターの接続ピン2
     * @param enc1 エンコーダの接続ピン1
     * @param enc2 エンコーダの接続ピン2
     */
    EJ_EncoderMotor(uint8_t pin1 = 0, uint8_t pin2 = 0, uint8_t enc1 = 0, uint8_t enc2 = 0);

    friend class EJ_EncoderMotor_Manager;

public:
    /**
     * @brief EJ_EncoderMotor クラスのデストラクタ
     */
    ~EJ_EncoderMotor();

public:
    /**
     * @brief 現在位置から相対移動
     * @param relativePosition 現在位置からの相対位置
     */
    void move(long relativePosition);

    /**
     * @brief 絶対位置まで絶対移動
     * @param absolutePosition 目標位置
     */
    void moveTo(long absolutePosition);
    
    /**
     * @brief 目標位置を設定する
     * @param targetPosition 目標位置
     */
    void setTarget(long targetPosition);

    /**
     * @brief 目標位置に到達したかどうか判定する
     * @return true: 目標位置に到達 / false: 目標位置に未達
     */
    bool isTargetReached();

private:
    static const char* _classname;
    uint8_t _pin1;
    uint8_t _pin2;
    uint8_t _enc1;
    uint8_t _enc2;
    long _target;
};

/**
 * @brief EJ_EncoderMotorクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class EJ_EncoderMotor_Manager
{
private:
    /**
     * @brief EJ_EncoderMotor_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_EncoderMotorの最大インスタンス数の定義
     */
    EJ_EncoderMotor_Manager(size_t maxInstanceSize);

private:
    /**
     * @brief EJ_EncoderMotor_Manager のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return EJ_EncoderMotor_Managerのsingletonを指すポインタ
     */
    static EJ_EncoderMotor_Manager* getInstance();

public:
    /**
     * @brief EJ_EncoderMotor_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_EncoderMotorクラスのインスタンスをすべて解放する
     */
    ~EJ_EncoderMotor_Manager();

public:
    /**
     * @brief EJ_EncoderMotor_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_EncoderMotorの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_EncoderMotorクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetMotor関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param MotorDef 参照
     * @return EJ_EncoderMotorクラスのインスタンスを指すポインタ
     */
    static EJ_EncoderMotor *createEncoderMotor(EncoderMotorDef motor);

    /**
     * @brief EJ_EncoderMotorクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetMotor関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param pin1 モータの接続ピン1
     * @param pin2 モータの接続ピン2
     * @param id モーターの識別番号
     * @return EJ_EncoderMotorクラスのインスタンスを指すポインタ
     */
    static EJ_EncoderMotor *createEncoderMotor(uint8_t pin1, uint8_t pin2, uint8_t enc1, uint8_t enc2, uint8_t id);

    /**
     * @brief EJ_EncoderMotorクラスのインスタンスを取得する
     * @details createMotor関数で生成したEJ_EncoderMotorクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param MotorDef 取得したいEncoderモータを指すEncoderモータ定義構造体
     * @return EJ_EncoderMotorクラスのインスタンスを指すポインタ
     */
    static EJ_EncoderMotor *getEncoderMotor(EncoderMotorDef motor);

    /**
     * @brief EJ_EncoderMotorクラスのインスタンスを取得する
     * @details createMotor関数で生成したEJ_EncoderMotorクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id モーターの識別番号
     * @return EJ_EncoderMotorクラスのインスタンスを指すポインタ
     */
    static EJ_EncoderMotor *getEncoderMotor(uint8_t id);

private:
    static const char* _classname;
    static EJ_EncoderMotor_Manager *_singleton;
    const size_t _maxInstanceSize;
    EJ_EncoderMotor **_instanceList;
};

#endif // EJENCODERMOTOR