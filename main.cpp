#include <cinderbox/cinderbox.hh>
#include <librapid/librapid.hpp>

#include <filesystem>
#include <queue>

// Interesting points to look at http://www.cuug.ab.ca/dewara/mandelbrot/Mandelbrowser.html

namespace lrc = librapid;

using Scalar = double; // Type used in all (most) calculations

struct RenderConfig {
	int64_t numThreads	  = 0;
	int64_t maxIterations = 0;
	int antiAlias		  = 0;
	lrc::Vec2i renderSize;
	lrc::Vec2i boxSize;
};

struct RenderBox {
	// Pixel space coordinates
	lrc::Vec2i origin;
	lrc::Vec2i size;
};

class FractalRenderer : public ci::app::App {
public:
	FractalRenderer() = default;

	void setup() override {
		// Main window settings
		setFrameRate(144);
		ci::gl::enableVerticalSync(true);

		// Setup ImGui
		ImGui::Initialize();
		ImGui::StyleColorsDark();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::GetIO().FontGlobalScale = 2.0f;

		// Setup fractal settings

		// Colors
		m_backgroundColor = ci::ColorA(0.1, 0.1, 0.1, 1);
		m_inSet			  = ci::ColorA(0.2, 0.2, 0.2, 1);

		// Minimum and maximum render size
		m_minRenderDimension = 10;
		m_maxRenderDimension = 20000;

		// Render and draw size
		m_drawSize = {800, 800};

		// Fractal space origin and size
		m_fractalSpaceOrigin   = {-0.765, 0.0};
		m_fractalSpaceSize	   = {2.47, 2.24};
		m_fractalOriginalScale = m_fractalSpaceSize;

		// Iteration settings
		m_iterationLowerBound = 1;
		m_iterationUpperBound = 100000000;

		// Used to track mouse dragging
		m_prevMouseDragPoint = {-1, -1};

		m_renderConfig = {
		  1,
		  1000,
		  1,
		  {800, 800},
		  {50, 50},
		};

		m_antiAliasLowerBound = 1;
		m_antiAliasUpperBound = 10;

		// We need a temporary one to avoid crashes on reloading
		m_tempRenderConfig = m_renderConfig;

		// Create render pool
		m_minThreads = 1;
		m_maxThreads = std::thread::hardware_concurrency();
		m_pool.reset(m_renderConfig.numThreads);

		// Start initial render
		regenerateSurface();
		queueRenderBoxes();
	}

	void cleanup() override {
		haltRenderBoxes();
		regenerateSurface();
	}

	void draw() override {
		// Clear the window
		ci::gl::clear(m_backgroundColor);

		// Draw ImGUI windows
		ImGui::Begin("Render Settings");
		{
			// Sliders for adjusting render resolution
			ImGui::DragScalarN("Render Resolution",
							   ImGuiDataType_S64,
							   &m_tempRenderConfig.renderSize.x,
							   2,
							   1,
							   &m_minRenderDimension,
							   &m_maxRenderDimension);

			// Sliders for adjusting draw scale
			ImGui::DragScalarN("Draw Size",
							   ImGuiDataType_S64,
							   &m_drawSize.x,
							   2,
							   1,
							   &m_minRenderDimension,
							   &m_maxRenderDimension);

			// Sliders to adjust render box size
			ImGui::DragScalarN("Render Box Size",
							   ImGuiDataType_S64,
							   &m_tempRenderConfig.boxSize.x,
							   2,
							   1,
							   &m_minRenderDimension,
							   &m_maxRenderDimension);

			// Slider for adjusting number of iterations
			ImGui::DragScalar("Max Iterations",
							  ImGuiDataType_S64,
							  &m_tempRenderConfig.maxIterations,
							  1,
							  &m_iterationLowerBound,
							  &m_iterationUpperBound);

			// Slider for adjusting anti aliasing factor
			ImGui::DragScalar("Anti Aliasing",
							  ImGuiDataType_S64,
							  &m_tempRenderConfig.antiAlias,
							  0.1,
							  &m_antiAliasLowerBound,
							  &m_antiAliasUpperBound);

			// Slider for adjusting number of threads used in render queue
			ImGui::DragScalar("Render Threads",
							  ImGuiDataType_S16,
							  &m_tempRenderConfig.numThreads,
							  0.1,
							  &m_minThreads,
							  &m_maxThreads);

			// Horizontal line
			ImGui::Separator();

			// Button to apply changes
			if (ImGui::Button("Apply Changes")) {
				m_renderConfig = m_tempRenderConfig;
				regenerateSurface();
				m_pool.reset(m_renderConfig.numThreads);
				queueRenderBoxes();
			}

			ImGui::SameLine();

			// Button to reset defaults
			if (ImGui::Button("Reset to Default")) {
				m_tempRenderConfig = {
				  1,
				  1000,
				  1,
				  {800, 800},
				  {50, 50},
				};

				m_renderConfig = m_tempRenderConfig;
				regenerateSurface();
				m_pool.reset(m_renderConfig.numThreads);
				queueRenderBoxes();
			}

			// Button to reset defaults
			if (ImGui::Button("Balls to the Walls")) {
				m_tempRenderConfig = {
				  30,
				  1000000,
				  4,
				  {1500, 1500},
				  {10, 10},
				};

				m_renderConfig = m_tempRenderConfig;
				regenerateSurface();
				m_pool.reset(m_renderConfig.numThreads);
				queueRenderBoxes();
			}

			// Button to reset defaults
			if (ImGui::Button("Halt Render")) { haltRenderBoxes(); }
		}
		ImGui::End();

		ImGui::Begin("File Export");
		{
			static std::string filename;
			ImGui::InputTextWithHint("Filename", "example: myImage.jpeg", &filename);

			if (!filename.empty() && ImGui::Button("Save")) {
				if (!saveImage(filename)) { CI_LOG_E("Failed to save image"); }
			}

			ImGui::Separator();

			ImGui::Text("Video Frame Export");
			static std::string videoDirectory;
			static std::string startPositionX;
			static std::string startPositionY;
			static std::string endPositionX;
			static std::string endPositionY;
			static std::string startZoom;
			static std::string endZoom;
			static int numFrames;
			ImGui::InputTextWithHint("Frame Directory", "example: myVideo", &videoDirectory);

			ImGui::Text("Start Position");
			ImGui::SameLine();
			if (ImGui::Button("Copy Current Position##start")) {
				startPositionX = lrc::str(m_fractalSpaceOrigin.x);
				startPositionY = lrc::str(m_fractalSpaceOrigin.y);
			}

			ImGui::InputTextWithHint("Start X", "starting x coordinate", &startPositionX);
			ImGui::InputTextWithHint("Start Y", "starting y coordinate", &startPositionY);

			ImGui::Text("End Position");
			ImGui::SameLine();
			if (ImGui::Button("Copy Current Position##end")) {
				endPositionX = lrc::str(m_fractalSpaceOrigin.x);
				endPositionY = lrc::str(m_fractalSpaceOrigin.y);
			}

			ImGui::InputTextWithHint("End X", "ending x coordinate", &endPositionX);
			ImGui::InputTextWithHint("End Y", "ending y coordinate", &endPositionY);

			ImGui::Text("Zoom");
			ImGui::InputTextWithHint("Start Zoom", "starting zoom", &startZoom);
			ImGui::InputTextWithHint("End Zoom", "ending zoom", &endZoom);
			ImGui::InputInt("Number of Frames", &numFrames, 2);

			if (!videoDirectory.empty() && !startPositionX.empty() && !startPositionY.empty() &&
				!endPositionX.empty() && !endPositionY.empty() && !startZoom.empty() &&
				!endZoom.empty() && ImGui::Button("Export Frames")) {
				Scalar startX, startY, endX, endY, startZ, endZ;

				bool valid = true;
				try {
					scn::scan(startPositionX, "{}", startX);
					scn::scan(startPositionY, "{}", startY);
					scn::scan(endPositionX, "{}", endX);
					scn::scan(endPositionY, "{}", endY);
					scn::scan(startZoom, "{}", startZ);
					scn::scan(endZoom, "{}", endZ);
				} catch (const scn::error &) {
					// Invalid input
					startPositionX = "";
					startPositionY = "";
					endPositionX   = "";
					endPositionY   = "";
					startZoom	   = "";
					endZoom		   = "";
					valid		   = false;
				}

				if (valid) {
					// Checks
					if (numFrames < 2) { numFrames = 2; }

					m_animate		 = true;
					m_videoDirectory = videoDirectory;
					m_frame			 = 0;
					m_numFrames		 = numFrames;
					m_videoStart	 = {startX, startY};
					m_videoEnd		 = {endX, endY};
					m_videoZoomStart = startZ;
					m_videoZoomEnd	 = endZ;

					std::filesystem::create_directories(m_videoDirectory);
					queueRenderBoxes();
				}
			}
		}
		ImGui::End();

		ImGui::Begin("High Precision Movement");
		{
			static std::string re, im;
			ImGui::Text("Fractal Origin");
			ImGui::SameLine();
			if (ImGui::Button("Copy Current Position")) {
				re = lrc::str(m_fractalSpaceOrigin.x);
				im = lrc::str(m_fractalSpaceOrigin.y);
			}
			ImGui::InputTextWithHint("Real", "real component", &re);
			ImGui::InputTextWithHint("Imag", "imaginary component", &im);

			if (!re.empty() && !im.empty() && ImGui::Button("Move")) {
				Scalar newX, newY;
				try {
					scn::scan(re, "{}", newX);
					scn::scan(im, "{}", newY);
				} catch (const scn::error &e) {
					// Invalid input
					re = "";
					im = "";
				}

				if (!re.empty() && !im.empty()) {
					m_fractalSpaceOrigin = {newX, newY};
					haltRenderBoxes();
					regenerateSurface();
					queueRenderBoxes();
				}
			}

			ImGui::Separator();

			ImGui::Text("Zoom");
			ImGui::SameLine();
			if (ImGui::Button("Copy Current Zoom")) {}

			static std::string zoomString;
			ImGui::Text("Fractal Zoom");
			ImGui::SameLine();

			if (ImGui::Button("Copy Current Zoom")) {
				auto zoom  = m_fractalOriginalScale / m_fractalSpaceSize;
				zoomString = lrc::str(zoom.x);
			}

			ImGui::InputTextWithHint("Zoom", "scale factor", &zoomString);

			if (!zoomString.empty() && ImGui::Button("Set Zoom")) {
				Scalar newZoom;
				try {
					scn::scan(zoomString, "{}", newZoom);
				} catch (const scn::error &e) {
					// Invalid input
					zoomString = "";
				}

				if (!zoomString.empty()) {
					m_fractalSpaceSize = m_fractalOriginalScale / newZoom;
					haltRenderBoxes();
					regenerateSurface();
					queueRenderBoxes();
				}
			}
		}
		ImGui::End();

		ImGui::Begin("Metrics");
		{
			float *fontScale = &ImGui::GetIO().FontGlobalScale;
			ImGui::InputFloat("GUI Scale", fontScale);

			ImGui::Separator();

			ImGui::Text("Frame Rate");
			double fps = getAverageFps();
			double spf = 1.0 / fps;
			ImGui::Text("%s", fmt::format("FPS: {:.2f}", fps).c_str());
			ImGui::Text("%s", fmt::format("SPF: {:.2f}ms", spf * 1000.0).c_str());

			ImGui::Text("Position");
			ImGui::Text("%s", fmt::format("X: {}", lrc::str(m_fractalSpaceOrigin.x)).c_str());
			ImGui::Text("%s", fmt::format("Y: {}", lrc::str(m_fractalSpaceOrigin.y)).c_str());

			auto zoom = m_fractalOriginalScale / m_fractalSpaceSize;
			ImGui::Text("%s", fmt::format("Zoom: {:e}x", zoom.x).c_str());

			ImGui::Separator();

			ImGui::Text("Thread Pool");
			ImGui::Text("%s", fmt::format("Total: {} items", m_pool.get_tasks_total()).c_str());
			ImGui::Text("%s", fmt::format("Running: {} items", m_pool.get_tasks_running()).c_str());
			ImGui::Text("%s", fmt::format("Queued: {} items", m_pool.get_tasks_queued()).c_str());

			ImGui::Separator();

			ImGui::Text("Video Export");
			ImGui::Text("%s", fmt::format("Animating: {}", m_animate).c_str());
			ImGui::Text("%s", fmt::format("Frame: {}", m_frame).c_str());
			ImGui::Text("%s", fmt::format("Total Frames: {}", m_numFrames).c_str());

			if (m_animate && ImGui::Button("Cancel Animation")) { m_animate = false; }
		}
		ImGui::End();

		// Render mandelbrot
		lrc::Vec2i frameSize = getWindowSize();
		m_fractalTexture	 = ci::gl::Texture2d::create(m_fractalSurface);
		lrc::Vec2i drawPos	 = frameSize / 2 - m_drawSize / 2;
		ci::gl::draw(m_fractalTexture, ci::Rectf(drawPos, drawPos + m_drawSize));

		if (m_animate) {
			// Video checks
			if (m_pool.get_tasks_total() == 0) {
				// Rendered current frame
				saveImage(fmt::format("{}/frame{:0>4}.png", m_videoDirectory, m_frame));

				// Next frame
				Scalar t = static_cast<Scalar>(m_frame) / static_cast<Scalar>(m_numFrames - 1);
				m_fractalSpaceOrigin = lrc::lerp(m_videoStart, m_videoEnd, t);
				m_fractalSpaceSize =
				  m_fractalOriginalScale / lrc::lerp(m_videoZoomStart, m_videoZoomEnd, t);

				m_frame++;

				if (m_frame == m_numFrames)
					m_animate = false;
				else
					queueRenderBoxes();
			}
		}
	}

	void regenerateSurface() {
		haltRenderBoxes(); // Must halt render before regenerating surface
		int64_t w		 = m_renderConfig.renderSize.x;
		int64_t h		 = m_renderConfig.renderSize.y;
		m_fractalSurface = ci::Surface((int32_t)w, (int32_t)h, true);
	}

	void haltRenderBoxes() {
		m_haltRender = true;
		m_pool.wait_for_tasks();
		m_haltRender = false;
	}

	void queueRenderBoxes() {
		// Create render boxes of WxH pixels
		int64_t w	   = m_renderConfig.renderSize.x;
		int64_t h	   = m_renderConfig.renderSize.y;
		int64_t boxW   = m_renderConfig.boxSize.x;
		int64_t boxH   = m_renderConfig.boxSize.y;
		int64_t boxesX = (w * boxW - 1) / boxW;
		int64_t boxesY = (h * boxH - 1) / boxH;

		for (int64_t y = 0; y < boxesY; y += boxH) {
			for (int64_t x = 0; x < boxesX; x += boxW) {
				RenderBox box {{x, y}, {lrc::min(x + boxW, w), lrc::min(y + boxH, h)}};
				m_pool.push_task([this, box]() { renderBox(box); });
			}
		}
	}

	void renderBox(const RenderBox &box) {
		if (m_haltRender) return;

		// If a perimeter is found to be in the set, the area bounded by that perimeter is also
		// contained in the set
		bool perimeter = true;

		// Top Edge
		for (int64_t px = box.origin.x; px < box.size.x && perimeter; ++px) {
			ci::ColorA pix = pixel(lrc::Vec2i(px, box.origin.y));
			if (pix != m_inSet) perimeter = false;
		}

		// Bottom Edge
		for (int64_t px = box.origin.x; px < box.size.x && perimeter; ++px) {
			ci::ColorA pix = pixel(lrc::Vec2i(px, box.size.y - 1));
			if (pix != m_inSet) perimeter = false;
		}

		// Left Edge
		for (int64_t py = box.origin.y; py < box.size.y && perimeter; ++py) {
			ci::ColorA pix = pixel(lrc::Vec2i(box.origin.x, py));
			if (pix != m_inSet) perimeter = false;
		}

		// Right Edge
		for (int64_t py = box.origin.y; py < box.size.y && perimeter; ++py) {
			ci::ColorA pix = pixel(lrc::Vec2i(box.size.x - 1, py));
			if (pix != m_inSet) perimeter = false;
		}

		if (perimeter) {
			for (int64_t py = box.origin.y; py < box.size.y; ++py) {
				for (int64_t px = box.origin.x; px < box.size.x; ++px) {
					m_fractalSurface.setPixel(lrc::Vec2i(px, py), m_inSet);
				}
			}
		} else {
			for (int64_t py = box.origin.y; py < box.size.y; ++py) {
				if (m_haltRender) return;
				for (int64_t px = box.origin.x; px < box.size.x; ++px) {
					m_fractalSurface.setPixel(lrc::Vec2i(px, py), pixel({px, py}));
				}
			}
		}
	}

	LR_INLINE ci::ColorA pixel(const lrc::Vec2i &pix) const {
		// Anti aliasing
		ci::ColorA col(0, 0, 0, 1);

		for (int i = 0; i < m_renderConfig.antiAlias; ++i) {
			for (int j = 0; j < m_renderConfig.antiAlias; ++j) {
				lrc::Vec<Scalar, 2> p0 = lrc::map(
				  lrc::Vec<Scalar, 2>(pix) + lrc::Vec<Scalar, 2>(i, j) / m_renderConfig.antiAlias,
				  lrc::Vec<Scalar, 2>(0, 0),
				  lrc::Vec<Scalar, 2>(m_renderConfig.renderSize.x - 1,
									  m_renderConfig.renderSize.x - 1),
				  m_fractalSpaceOrigin - m_fractalSpaceSize / 2,
				  m_fractalSpaceOrigin + m_fractalSpaceSize / 2);

				col += pointToColor(p0);
			}
		}

		return col / static_cast<float>(m_renderConfig.antiAlias * m_renderConfig.antiAlias);
	}

	LR_INLINE ci::ColorA pointToColor(const lrc::Vec<Scalar, 2> &p0) const {
		// Mandelbrot Calculation
		Scalar x0 = p0.x;
		Scalar y0 = p0.y;
		Scalar x = 0, y = 0;
		int64_t iteration = 0;

		double bailout = 1 << 16;

		while (x * x + y * y <= bailout && iteration < m_renderConfig.maxIterations) {
			Scalar tmp = x * x - y * y + x0;
			y		   = 2 * x * y + y0;
			x		   = tmp;
			++iteration;
		}

		ci::ColorA col;
		if (x * x + y * y <= 4) {
			col = m_inSet; // In set (probably)
		} else {
			auto floatingX = lrc::internal::traits<Scalar>::cast<double>(x);
			auto floatingY = lrc::internal::traits<Scalar>::cast<double>(y);

			double s1 = (double)iteration -
						lrc::log2(lrc::log2(floatingX * floatingX + floatingY * floatingY)) + 4;
			lrc::Vec3d colVec = 0.5 + 0.5 * lrc::cos(3.0 + s1 * 0.15 + lrc::Vec3d(0.0, 0.6, 1.0));
			col.r			  = (float)colVec.x;
			col.g			  = (float)colVec.y;
			col.b			  = (float)colVec.z;
			col.a			  = 1;
		}

		return col;
	}

	void mouseUp(ci::app::MouseEvent event) override {
		if (m_animate) return;
		m_prevMouseDragPoint = {-1, -1};
	}

	void mouseWheel(ci::app::MouseEvent event) override {
		if (m_animate) return;

		if (event.getWheelIncrement() > 0) {
			m_fractalSpaceSize *= 0.9;
			m_zoom /= 0.9;
		} else {
			m_fractalSpaceSize *= 1.1;
			m_zoom /= 1.1;
		}
		queueRenderBoxes();
	}

	void mouseDrag(ci::app::MouseEvent event) override {
		if (m_animate) return;

		if (m_prevMouseDragPoint.x < 0) { m_prevMouseDragPoint = event.getPos(); }

		lrc::Vec3i current = event.getPos();
		lrc::Vec3i delta   = current - m_prevMouseDragPoint;

		lrc::Vec<Scalar, 2> ratio =
		  lrc::Vec<Scalar, 2>(delta) / lrc::Vec<Scalar, 2>(m_renderConfig.renderSize);
		lrc::Vec<Scalar, 2> adjusted = ratio * m_fractalSpaceSize;
		m_fractalSpaceOrigin -= adjusted;

		m_prevMouseDragPoint = current;
		queueRenderBoxes();
	}

	bool saveImage(const std::string &filename) const {
		if (filename.empty()) return false;
		ci::writeImage(filename, m_fractalSurface);
		return true;
	}

private:
	// Surface and texture used to render the fractal
	ci::Surface m_fractalSurface;
	ci::gl::Texture2dRef m_fractalTexture;

	// Colors
	ci::ColorA m_backgroundColor;
	ci::ColorA m_inSet;

	// Thread pool to render the fractal (CPU only)
	int64_t m_minThreads = 0;
	int64_t m_maxThreads = 0;
	bool m_haltRender	 = false;
	lrc::ThreadPool m_pool;

	// Thread to update the actual texture
	std::thread m_textureThread;

	// Minimum and maximum render size
	int64_t m_minRenderDimension = 0;
	int64_t m_maxRenderDimension = 0;

	// Size to draw the fractal
	lrc::Vec2i m_drawSize;

	// Fractal space origin and size
	lrc::Vec<Scalar, 2> m_fractalSpaceOrigin;
	lrc::Vec<Scalar, 2> m_fractalSpaceSize;
	lrc::Vec<Scalar, 2> m_fractalOriginalScale;

	// Iteration bounds
	int64_t m_iterationLowerBound = 0;
	int64_t m_iterationUpperBound = 0;

	int64_t m_antiAliasLowerBound = 0;
	int64_t m_antiAliasUpperBound = 0;

	// Zoom info
	double m_zoom = 0;

	// Used to track mouse dragging
	lrc::Vec2i m_prevMouseDragPoint;

	RenderConfig m_renderConfig;
	RenderConfig m_tempRenderConfig;

	// Video export
	bool m_animate = false;
	std::string m_videoDirectory;
	int64_t m_frame		= 0;
	int64_t m_numFrames = 0;
	lrc::Vec<Scalar, 2> m_videoStart;
	lrc::Vec<Scalar, 2> m_videoEnd;
	Scalar m_videoZoomStart = 0;
	Scalar m_videoZoomEnd	= 0;
};

CINDER_APP(FractalRenderer, ci::app::RendererGl(ci::app::RendererGl::Options().msaa(4)))
