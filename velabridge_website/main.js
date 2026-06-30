"use strict";

const prefersReducedMotion = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

const translations = {
  zh: {
    navProduct: "产品",
    navTechnology: "技术",
    navDemoShort: "演示",
    navProof: "实证",
    navTeam: "团队",
    navDemo: "查看演示",
    heroSubtitle: "基于 openvela 的 AI 无障碍智能手表。",
    heroCopy: "让字幕、识别、提醒与回应，在腕上完成。",
    heroPrimary: "观看演示",
    heroSecondary: "查看技术实证",
    visionEyebrow: "产品愿景",
    visionTitle: "连接感知与回应。",
    visionCopy: "面向真实无障碍场景的轻量 AI 交互层。",
    audience1Title: "听障用户",
    audience1Copy: "把外界语音转成可快速阅读的字幕，降低沟通压力。",
    audience2Title: "视障用户",
    audience2Copy: "把环境文字和简单提示转成语音、震动或可理解反馈。",
    audience3Title: "老年用户",
    audience3Copy: "用克制、明确的腕上提醒，辅助窗口服务、问路和点餐。",
    featuresEyebrow: "核心能力",
    featuresTitle: "把复杂信号，变成清晰回应。",
    feature1Title: "实时字幕",
    feature1Copy: "把语音实时转成可读字幕。",
    feature1Output: "老师说：请先到教务处服务窗口",
    feature2Title: "OCR 识别",
    feature2Copy: "把环境文字变成可行动信息。",
    feature2Output: "食堂一号窗口，今日套餐 15 元",
    feature3Title: "危险提醒",
    feature3Copy: "把空间线索转成及时提醒。",
    feature3Output: "检测到前方台阶，请小心",
    feature4Title: "快捷回复",
    feature4Copy: "在合适时刻给出简单、明确的回应。",
    feature4Output: "请您说慢一点，我正在看字幕",
    techEyebrow: "技术链路",
    techTitle: "从传感输入，到无障碍回应。",
    arch1Title: "用户",
    arch1Small: "需求",
    arch2Title: "手表传感",
    arch2Small: "输入",
    arch3Small: "状态机",
    arch4Small: "模拟推理",
    arch5Title: "回应",
    arch5Small: "字幕 · 语音 · 震动",
    proofEyebrow: "真实硬件实证",
    proofTitle: "已在真实开发板验证。",
    specBoard: "开发板",
    specOS: "系统",
    specApp: "应用",
    specFlash: "烧录地址",
    specShell: "终端",
    specStatus: "状态",
    specStatusValue: "运行中",
    demoEyebrow: "手表演示",
    demoTitle: "在关键时刻，给出冷静清晰的界面。",
    watchTag1: "字幕",
    watchCard1: "老师说：请先到教务处服务窗口",
    watchCard2: "食堂一号窗口，今日套餐 15 元",
    watchTag3: "危险",
    watchCard3: "检测到前方台阶，请小心",
    watchHint: "openvela · AI Bridge mock 就绪",
    brandEyebrow: "品牌识别",
    brandCopy: "为智能无障碍而生的安静科技品牌。",
    teamEyebrow: "团队",
    teamFact2: "真实开发板验证",
    teamFact3: "基于 openvela 构建",
    footerCopy: "连接智能与无障碍。"
  },
  en: {
    navProduct: "Product",
    navTechnology: "Technology",
    navDemoShort: "Demo",
    navProof: "Proof",
    navTeam: "Team",
    navDemo: "View Demo",
    heroSubtitle: "An AI accessibility watch built on openvela.",
    heroCopy: "Captions, recognition, alerts and replies, completed on the wrist.",
    heroPrimary: "Watch Demo",
    heroSecondary: "View Technical Proof",
    visionEyebrow: "Product Vision",
    visionTitle: "Bridging perception and response.",
    visionCopy: "A lightweight AI layer for real-world accessibility.",
    audience1Title: "Hearing impaired",
    audience1Copy: "Speech becomes concise visual information that can be read at a glance.",
    audience2Title: "Visually impaired",
    audience2Copy: "Environmental text and simple cues become spoken, tactile or readable prompts.",
    audience3Title: "Elderly users",
    audience3Copy: "Calm wrist-level reminders support services, directions and ordering.",
    featuresEyebrow: "Core Features",
    featuresTitle: "Accessibility signals, delivered clearly.",
    feature1Title: "Live Caption",
    feature1Copy: "Speech becomes readable in real time.",
    feature1Output: "Teacher: please go to the academic service desk.",
    feature2Title: "OCR Vision",
    feature2Copy: "Text in the environment becomes actionable.",
    feature2Output: "Cafeteria Window 1: today's meal is 15 yuan.",
    feature3Title: "Danger Alert",
    feature3Copy: "Spatial cues become timely warnings.",
    feature3Output: "Step detected ahead, please be careful.",
    feature4Title: "Quick Reply",
    feature4Copy: "A simple response, delivered at the right moment.",
    feature4Output: "Please speak slower, I am reading captions.",
    techEyebrow: "Technology",
    techTitle: "From sensor input to accessible response.",
    arch1Title: "User",
    arch1Small: "need",
    arch2Title: "Watch Sensor",
    arch2Small: "input",
    arch3Small: "state",
    arch4Small: "mock",
    arch5Title: "Response",
    arch5Small: "caption · voice · haptic",
    proofEyebrow: "Real Board Proof",
    proofTitle: "Verified on real hardware.",
    specBoard: "Board",
    specOS: "OS",
    specApp: "App",
    specFlash: "Flash",
    specShell: "Shell",
    specStatus: "Status",
    specStatusValue: "Running",
    demoEyebrow: "Watch Demo",
    demoTitle: "A calm interface for urgent moments.",
    watchTag1: "Caption",
    watchCard1: "Teacher: please go to the academic service desk.",
    watchCard2: "Cafeteria Window 1: today's meal is 15 yuan.",
    watchTag3: "Danger",
    watchCard3: "Step detected ahead, please be careful.",
    watchHint: "openvela · mock bridge ready",
    brandEyebrow: "Brand Identity",
    brandCopy: "A quiet technology identity for intelligent accessibility.",
    teamEyebrow: "Team",
    teamFact2: "Real-board validated",
    teamFact3: "Built with openvela",
    footerCopy: "Bridging intelligence and accessibility."
  }
};

function applyLanguage(language) {
  const dictionary = translations[language] || translations.zh;
  document.documentElement.lang = language === "zh" ? "zh-CN" : "en";
  document.querySelectorAll("[data-i18n]").forEach((element) => {
    const key = element.dataset.i18n;
    if (dictionary[key]) {
      element.textContent = dictionary[key];
    }
  });

  const toggle = document.querySelector("[data-language-toggle]");
  if (toggle) {
    toggle.textContent = language === "zh" ? "EN" : "中";
    toggle.setAttribute(
      "aria-label",
      language === "zh" ? "Switch to English" : "切换到中文"
    );
  }

  window.localStorage.setItem("velabridge-language", language);
}

function initLanguageToggle() {
  const toggle = document.querySelector("[data-language-toggle]");
  const saved = window.localStorage.getItem("velabridge-language");
  const initial = saved === "en" ? "en" : "zh";
  applyLanguage(initial);

  if (!toggle) {
    return;
  }

  toggle.addEventListener("click", () => {
    const next = document.documentElement.lang === "zh-CN" ? "en" : "zh";
    applyLanguage(next);
  });
}

function initProgressBar() {
  const progress = document.querySelector(".scroll-progress");
  const header = document.querySelector("[data-header]");

  function update() {
    const max = document.documentElement.scrollHeight - window.innerHeight;
    const ratio = max > 0 ? window.scrollY / max : 0;
    progress.style.width = `${Math.min(ratio * 100, 100)}%`;
    header.classList.toggle("is-scrolled", window.scrollY > 24);
  }

  update();
  window.addEventListener("scroll", update, { passive: true });
  window.addEventListener("resize", update);
}

function initParticles() {
  const canvas = document.getElementById("particleCanvas");
  if (!canvas || prefersReducedMotion) {
    return;
  }

  const context = canvas.getContext("2d");
  const pointer = { x: 0, y: 0 };
  let particles = [];
  let width = 0;
  let height = 0;
  let pixelRatio = 1;

  function resize() {
    pixelRatio = Math.min(window.devicePixelRatio || 1, 2);
    width = window.innerWidth;
    height = window.innerHeight;
    canvas.width = Math.floor(width * pixelRatio);
    canvas.height = Math.floor(height * pixelRatio);
    canvas.style.width = `${width}px`;
    canvas.style.height = `${height}px`;
    context.setTransform(pixelRatio, 0, 0, pixelRatio, 0, 0);

    const count = Math.min(92, Math.max(42, Math.floor(width * height / 23000)));
    particles = Array.from({ length: count }, () => ({
      x: Math.random() * width,
      y: Math.random() * height,
      vx: (Math.random() - 0.5) * 0.22,
      vy: (Math.random() - 0.5) * 0.22,
      r: Math.random() * 1.4 + 0.4,
      a: Math.random() * 0.38 + 0.12
    }));
  }

  function render() {
    context.clearRect(0, 0, width, height);
    context.fillStyle = "rgba(42, 123, 255, 0.45)";

    particles.forEach((particle, index) => {
      particle.x += particle.vx + pointer.x * 0.006;
      particle.y += particle.vy + pointer.y * 0.006;

      if (particle.x < -20) particle.x = width + 20;
      if (particle.x > width + 20) particle.x = -20;
      if (particle.y < -20) particle.y = height + 20;
      if (particle.y > height + 20) particle.y = -20;

      context.globalAlpha = particle.a;
      context.beginPath();
      context.arc(particle.x, particle.y, particle.r, 0, Math.PI * 2);
      context.fill();

      for (let j = index + 1; j < particles.length; j += 1) {
        const other = particles[j];
        const dx = particle.x - other.x;
        const dy = particle.y - other.y;
        const distance = Math.sqrt(dx * dx + dy * dy);
        if (distance < 118) {
          context.globalAlpha = (1 - distance / 118) * 0.13;
          context.strokeStyle = "rgba(42, 123, 255, 1)";
          context.lineWidth = 1;
          context.beginPath();
          context.moveTo(particle.x, particle.y);
          context.lineTo(other.x, other.y);
          context.stroke();
        }
      }
    });

    context.globalAlpha = 1;
    requestAnimationFrame(render);
  }

  window.addEventListener("resize", resize);
  window.addEventListener("pointermove", (event) => {
    pointer.x = (event.clientX / window.innerWidth - 0.5) * 2;
    pointer.y = (event.clientY / window.innerHeight - 0.5) * 2;
  }, { passive: true });

  resize();
  render();
}

function initScrollReveal() {
  const items = document.querySelectorAll(".reveal");
  if (!items.length) {
    return;
  }

  document.body.classList.add("motion-ready");

  if (prefersReducedMotion) {
    items.forEach((item) => item.classList.add("is-visible"));
    return;
  }

  const observer = new IntersectionObserver((entries) => {
    entries.forEach((entry) => {
      if (entry.isIntersecting) {
        entry.target.classList.add("is-visible");
        observer.unobserve(entry.target);
      }
    });
  }, { threshold: 0.16, rootMargin: "0px 0px -8% 0px" });

  items.forEach((item) => observer.observe(item));
}

function initTiltCards() {
  if (prefersReducedMotion) {
    return;
  }

  document.querySelectorAll(".tilt-card").forEach((card) => {
    card.addEventListener("pointermove", (event) => {
      const rect = card.getBoundingClientRect();
      const x = (event.clientX - rect.left) / rect.width - 0.5;
      const y = (event.clientY - rect.top) / rect.height - 0.5;
      card.style.transform = `perspective(900px) rotateX(${-y * 5}deg) rotateY(${x * 6}deg) translateY(-2px)`;
    });

    card.addEventListener("pointerleave", () => {
      card.style.transform = "";
    });
  });
}

function initWatchDemo() {
  const cards = Array.from(document.querySelectorAll("[data-watch-card]"));
  if (!cards.length || prefersReducedMotion) {
    return;
  }

  let index = 0;
  window.setInterval(() => {
    cards[index].classList.remove("is-active");
    index = (index + 1) % cards.length;
    cards[index].classList.add("is-active");
  }, 2800);
}

function initArchitectureSequence() {
  const map = document.querySelector("[data-architecture]");
  const nodes = Array.from(document.querySelectorAll(".arch-node"));
  if (!map || !nodes.length || prefersReducedMotion) {
    nodes.forEach((node) => node.classList.add("is-lit"));
    return;
  }

  let timer = null;
  const observer = new IntersectionObserver((entries) => {
    entries.forEach((entry) => {
      if (!entry.isIntersecting || timer) {
        return;
      }

      let index = 0;
      timer = window.setInterval(() => {
        nodes.forEach((node, nodeIndex) => {
          node.classList.toggle("is-lit", nodeIndex <= index);
        });
        index += 1;
        if (index > nodes.length) {
          window.clearInterval(timer);
        }
      }, 320);
    });
  }, { threshold: 0.35 });

  observer.observe(map);
}

function initParallax() {
  const target = document.querySelector("[data-parallax]");
  if (!target || prefersReducedMotion) {
    return;
  }

  window.addEventListener("pointermove", (event) => {
    const x = (event.clientX / window.innerWidth - 0.5) * 10;
    const y = (event.clientY / window.innerHeight - 0.5) * 10;
    target.style.transform = `translate3d(${x}px, ${y}px, 0)`;
  }, { passive: true });
}

function initBrandImageFallback() {
  const images = Array.from(document.querySelectorAll(".brand-image"));
  if (!images.length) {
    document.body.classList.add("no-brand-image");
    return;
  }

  let loaded = false;
  let settled = 0;

  images.forEach((image) => {
    const markSettled = () => {
      settled += 1;
      if (settled === images.length) {
        document.body.classList.toggle("has-brand-image", loaded);
        document.body.classList.toggle("no-brand-image", !loaded);
      }
    };

    image.addEventListener("load", () => {
      loaded = true;
      markSettled();
    }, { once: true });

    image.addEventListener("error", markSettled, { once: true });

    if (image.complete) {
      if (image.naturalWidth > 0) {
        loaded = true;
      }
      markSettled();
    }
  });
}

function init() {
  initLanguageToggle();
  initBrandImageFallback();
  initProgressBar();
  initParticles();
  initScrollReveal();
  initTiltCards();
  initWatchDemo();
  initArchitectureSequence();
  initParallax();
}

document.addEventListener("DOMContentLoaded", init);
