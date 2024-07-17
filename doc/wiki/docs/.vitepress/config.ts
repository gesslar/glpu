import { defineConfig } from 'vitepress'
import { SIDEBAR } from "./sidebar";
import { viteStaticCopy } from "vite-plugin-static-copy";

// https://vitepress.vuejs.org/config/app-configs
export default defineConfig({
  title: "gLPU",
  description: "Documentation repository for the gLPU project",
  base: "/", // Base URL for your site
  themeConfig: {
    sidebar: SIDEBAR,
  },
});
