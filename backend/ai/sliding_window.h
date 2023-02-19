#include <array>
#include <cassert>
#include <iostream>

template <int window_size, class T>
class CircularBuffer {
	public:
		void init(T v) {
			for (int i=0; i<window_size; ++i) {
				window[i] = v;
			}
			idx = 0;
		}

		T back() const {
			if (idx == window_size-1) {
				return window[0];
			} else {
				return window[idx+1];
			}
		}

		void push(T t) {
			window[idx] = t;
			++idx;
			if (idx >= window_size) {
				idx = 0;
			}
		}


	private:
		std::array<T, window_size> window{};
		unsigned int idx = 0;
};

template <int window_size>
class HitCountSlidingWindow {
	public:
		HitCountSlidingWindow() {
			window.init(false);
			hit_count = 0;
		}

		constexpr int size() const { return window_size; }
		
		void add(bool hit) {
			const bool popped = window.back();
			if (popped) {
				assert(hit_count>0);
				--hit_count;
			}

			window.push(hit);
			if (hit) {
				++hit_count;
			}
		}

		int getHitCount() const { return hit_count; }

	private:
		CircularBuffer<window_size, bool> window;
		int hit_count = 0;
};
