import { defineConfig } from 'vitepress'

// https://vitepress.vuejs.org/config/app-configs
export default defineConfig({
  title: "gLPU",
  description: "Documentation repository for the gLPU project",
  base: "/", // Base URL for your site
  themeConfig: {
    nav: [
      { text: "Home", link: "/" },
      { text: "FluffOS", link: "https://fluffos.info" },
    ],
    sidebar: [
      {
        collapsed: false,
        text: "Functions",
        items: [
          { text: "Simul Efuns", link: "/simul_efun/" },
          { text: "Overridden Efuns", link: "/efun_override/" },
        ],
      },
    ],
    socialLinks: [
      { icon: "github", link: "https://github.com/gesslar/glpu" },
      { icon: "discord", link: "https://discord.gg/wzUbBgs3AQ" }
    ],
    externalLinkIcon: false,
  },
});
