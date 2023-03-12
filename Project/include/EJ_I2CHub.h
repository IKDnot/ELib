/**
 * @file           EJ_I2CHub.h
 * @brief          PaHubユニット(PCA9548AP)を制御するEJ_I2CHubクラスと、EJ_I2CHubクラスを管理するEJ_I2CHub_Managerクラスの定義
 * @author         IKDnot
 * @date           2023/03/12
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

#ifndef EJI2CHub
#define EJI2CHub
#include <Arduino.h>
#include "ClosedCube_TCA9548A.h"

/**
 * @brief ToFセンサユニットを制御するクラス
 * @details *注意:本クラスのインスタンスはEJ_I2CHub_Managerクラス以外からは生成できない
 */
class EJ_I2CHub : public ClosedCube::Wired::TCA9548A
{
private:
    /**
     * @brief EJ_I2CHubクラスのコンストラクタ
     * @param address I2C通信で利用するアドレス
     */
    EJ_I2CHub(uint8_t address);

    friend class EJ_I2CHub_Manager;

public:
    /**
     * @brief EJ_I2CHubクラスのデストラクタ
     */
    ~EJ_I2CHub();

private:
    static const char* _classname;
    uint8_t _address;
};

/**
 * @brief EJ_I2CHubクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class EJ_I2CHub_Manager
{
private:
    /**
     * @brief EJ_I2CHub_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_I2CHubの最大インスタンス数の定義
     */
    EJ_I2CHub_Manager(size_t maxInstanceSize);

private:
    /**
     * @brief EJ_I2CHub_Manager のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return EJ_I2CHub_Managerのsingletonを指すポインタ
     */
    static EJ_I2CHub_Manager* getInstance();

public:
    /**
     * @brief EJ_I2CHub_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_I2CHubクラスのインスタンスをすべて解放する
     */
    ~EJ_I2CHub_Manager();

public:
    /**
     * @brief EJ_I2CHub_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_I2CHubの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_I2CHubクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetI2CHub関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param id PaHubユニットの識別番号
     * @param address PaHubユニット自身のI2Cアドレス (default: 0x70)
     * @return EJ_I2CHubクラスのインスタンスを指すポインタ
     */
    static EJ_I2CHub *createI2CHub(uint8_t id, uint8_t address = 0x70);

    /**
     * @brief EJ_I2CHubクラスのインスタンスを取得する
     * @details createI2CHub関数で生成したEJ_I2CHubクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id PaHubユニットの識別番号
     * @return EJ_I2CHubクラスのインスタンスを指すポインタ
     */
    static EJ_I2CHub *getI2CHub(uint8_t id);

private:
    static const char* _classname;
    static EJ_I2CHub_Manager *_singleton;
    const size_t _maxInstanceSize;
    EJ_I2CHub **_instanceList;
};

#endif // EJI2CHub