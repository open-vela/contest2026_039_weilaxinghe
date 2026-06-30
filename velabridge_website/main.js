"use strict";

const prefersReducedMotion = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

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
