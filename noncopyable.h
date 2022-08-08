#pragma once
/**
 * @brief:继承该类的子类不允许拷贝构造和拷贝赋值
 *
 */
class noncopyable
{
public:
	// 将拷贝构造和赋值构造delete掉
	noncopyable(const noncopyable &) = delete;
	noncopyable &operator=(const noncopyable &) = delete;

protected:
	noncopyable() = default;
	~noncopyable() = default;
};