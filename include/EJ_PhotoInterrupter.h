/**
 * @file           EJ_PhotoInterrupter.h
 * @brief          フォトインタラプタを制御するクラスEJ_PhotoInterrupterクラスと、EJ_PhotoInterrupterクラスを管理するEJ_PhotoInterrupter_Managerクラスの定義
 * @author         IKDnot
 * @date           2023/03/19
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

#ifndef EJPHOTOINTERRUPTER
#define EJPHOTOINTERRUPTER
#include <Arduino.h>

/**
 * @struct PhotoInterrupterDef
 * @brief 1つのフォトインタラプタを定義する構造体
 */
typedef struct
{
    uint8_t pin; /**< フォトインタラプタの接続ピン */
    uint8_t id;   /**< フォトインタラプタ識別番号 */
} PhotoInterrupterDef;

/**
 * @brief フォトインタラプタを制御するクラス
 * @details *注意:本クラスのインスタンスはEJ_PhotoInterrupter_Managerクラス以外からは生成できない
 */
class EJ_PhotoInterrupter
{
protected:
    /**
     * @brief EJ_PhotoInterrupterクラスのコンストラクタ
     * @param pin1 フォトインタラプタの接続ピン
     */
    EJ_PhotoInterrupter(uint8_t pin = 0);

    friend class EJ_PhotoInterrupter_Manager;

private:

public:
    /**
     * @brief EJ_PhotoInterrupter クラスのデストラクタ
     */
    ~EJ_PhotoInterrupter();

public:
    /**
     * @brief フォトインタラプタの発光部/受光部間の物体有無 (≒遮断されたか) を判定する
     * @return true: 遮断された / false: 遮断されていない
     */
    bool isInterrupted();

private:
    static const char* _classname;
    uint8_t _pin;
};

/**
 * @brief EJ_PhotoInterrupterクラスのインスタンスを生成、管理するクラス
 * @details 本クラスはシングルトンで提供され、configure()メソッドにのみよってインスタンスが1つだけ生成される
 */
class EJ_PhotoInterrupter_Manager
{
private:
    /**
     * @brief EJ_PhotoInterrupter_Managerクラスのコンストラクタ
     * @param maxInstanceSize EJ_PhotoInterrupterの最大インスタンス数の定義
     */
    EJ_PhotoInterrupter_Manager(size_t maxInstanceSize);

private:
    /**
     * @brief EJ_PhotoInterrupter_Manager のインスタンスを取得する
     * @attention *configure()がコールされていない場合はNULLポインタを返す
     * @return EJ_PhotoInterrupter_Managerのsingletonを指すポインタ
     */
    static EJ_PhotoInterrupter_Manager* getInstance();

public:
    /**
     * @brief EJ_PhotoInterrupter_Managerクラスのデストラクタ
     * @details 本クラスによって生成されたEJ_PhotoInterrupterクラスのインスタンスをすべて解放する
     */
    ~EJ_PhotoInterrupter_Manager();

public:
    /**
     * @brief EJ_PhotoInterrupter_Managerのインスタンスを生成する
     * @param maxInstanceSize EJ_PhotoInterrupterの最大インスタンス数の定義
     * @return true: 生成成功 / false: 生成失敗
     */
    static bool configure(size_t maxInstanceSize);

    /**
     * @brief EJ_PhotoInterrupterクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetPhotoInterrupter関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param PhotoInterrupterDef 参照
     * @return EJ_PhotoInterrupterクラスのインスタンスを指すポインタ
     */
    static EJ_PhotoInterrupter *createPhotoInterrupter(PhotoInterrupterDef PhotoInterrupter);

    /**
     * @brief EJ_PhotoInterrupterクラスのインスタンスを生成する
     * @details 生成したインスタンスはgetPhotoInterrupter関数で取得できるように同時に自身の_instanceList配列に記憶しておく
     * @param pin フォトインタラプタの接続ピン
     * @param id フォトインタラプタの識別番号
     * @return EJ_PhotoInterrupterクラスのインスタンスを指すポインタ
     */
    static EJ_PhotoInterrupter *createPhotoInterrupter(uint8_t pin, uint8_t id);

    /**
     * @brief EJ_PhotoInterrupterクラスのインスタンスを取得する
     * @details createPhotoInterrupter関数で生成したEJ_PhotoInterrupterクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param PhotoInterrupterDef 取得したいフォトインタラプタを指すフォトインタラプタ定義構造体
     * @return EJ_PhotoInterrupterクラスのインスタンスを指すポインタ
     */
    static EJ_PhotoInterrupter *getPhotoInterrupter(PhotoInterrupterDef PhotoInterrupter);

    /**
     * @brief EJ_PhotoInterrupterクラスのインスタンスを取得する
     * @details createPhotoInterrupter関数で生成したEJ_PhotoInterrupterクラスのインスタンスを取得し返す。指定したidのインスタンスが生成されていない場合や取得に失敗した場合はNULLを返す。
     * @param id フォトインタラプタの識別番号
     * @return EJ_PhotoInterrupterクラスのインスタンスを指すポインタ
     */
    static EJ_PhotoInterrupter *getPhotoInterrupter(uint8_t id);

private:
    static const char* _classname;
    static EJ_PhotoInterrupter_Manager *_singleton;
    const size_t _maxInstanceSize;
    EJ_PhotoInterrupter **_instanceList;
};

#endif // EJPHOTOINTERRUPTER